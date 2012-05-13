/* Copyright (C) 2011 CROZET Sébastien

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include "stdafx.hh"
#include "PairManager.hh"

namespace Falling
{
    Pair::Pair(ushort i1, ushort i2) : id1(i1), id2(i2), e(0)
    { }


    PairManager::PairManager(void (*f)(Pair &p))
    {
	hashTable = new int[32];
	next = new int[32];
	table = new Pair[32];
	for(int i = 0; i < 32; i++)
	{
	    hashTable[i] = -1;
	    next[i] = -1;
	}
	mask = 31; // mask = 0000 0000 0000 0000 0000 0000 0001 1111b
	maxPairs = 32;
	nbPairs = 0;
	realmaxPairs = (int) (32 * HASH_CHARGE_FACTOR);
	if(f)
	    c_delete = f;
	else
	    c_delete = nop;
    }

    PairManager::~PairManager()
    {
	delete []next;
	delete []hashTable;
	for(int i = 0; i < nbPairs; i++)
	    c_delete(table[i]);
	delete []table;
    }

    void PairManager::nop(Pair &)
    { } // do nothing


    Pair *PairManager::findPair(ushort id1, ushort id2)
    {
	orderIDs(id1, id2);
	uint h = hash(id1, id2) & mask;
	int o = hashTable[h];
	if(o != -1 && !table[o].equals(id1, id2))
	{
	    while(next[o] != -1 && !table[next[o]].equals(id1, id2))
		o = next[o];
	    o = next[o];
	}
	return (o == -1) ? 0 : &table[o];
    }

    Pair *PairManager::findPair(ushort id1, ushort id2, uint h)
    {
	int o = hashTable[h];
	if(o != -1 && !table[o].equals(id1, id2))
	{
	    while(next[o] != -1 && !table[next[o]].equals(id1, id2))
		o = next[o];
	    o = next[o];
	}
	return (o == -1) ? 0 : &table[o];
    }
    Pair *PairManager::addPair(ushort id1, ushort id2)
    {
	orderIDs(id1, id2);
	uint h = hash(id1, id2);
	Pair * f = findPair(id1, id2, h & mask);
	if(!f)
	{
	    if(nbPairs >= realmaxPairs) // realmaxPairs = maxPairs * HASH_CHARGE_FACTOR => keep some pourcent of free memory on hash table (to avoid too many collisions)
	    {
		Pair *newTable;
		int *newHash, *newNext;
		// resize & update
		maxPairs *= 2;			// next power of two
		int ormax = realmaxPairs;
		realmaxPairs = (int) (maxPairs * HASH_CHARGE_FACTOR);
		mask = maxPairs - 1;    // modulo &
		newHash = new int[maxPairs];
		newTable = new Pair[realmaxPairs];
		newNext = new int[maxPairs];
		for(int i = 0; i < maxPairs; i++)
		{
		    newHash[i] = -1;
		    newNext[i] = -1;
		}
		memcpy(newTable, table, ormax*sizeof(Pair)); // cpy Pair datas
		// create new pair
		// fill new hash table (because mask changed)
		for(int i = 0; i < nbPairs; i++)
		{

		    int idh = hash(newTable[i].id1, newTable[i].id2) & mask;
		    newNext[i] = newHash[idh];
		    newHash[idh] = i;
		}
		delete []hashTable;
		delete []table;
		delete []next;
		hashTable = newHash;
		table = newTable;
		next = newNext;
	    }
	    int test = h & mask;
	    table[nbPairs] = Pair(id1, id2);
	    next[nbPairs] = hashTable[test];
	    hashTable[test] = nbPairs;
	    nbPairs++;
	    return &table[nbPairs - 1];
	}
	else
	    return f;
    }

    bool PairManager::removePair(Pair *p)
    {
	return removePair(p->id1, p->id2);
    }

    bool PairManager::removePair(ushort id1, ushort id2)
    {
	orderIDs(id1, id2);
	uint h = hash(id1, id2) & mask;
	// find object
	int obji = 0;
	int o = hashTable[h];
	if(o != -1)
	{
	    if(!table[o].equals(id1, id2))
	    {
		while(next[o] != -1 && !table[next[o]].equals(id1, id2))
		    o = next[o];
		if(next[o] == -1)
		    return false;
		obji = next[o];
		next[o] = next[obji];
		next[obji] = -1;
	    }
	    else
	    {
		obji = o;
		hashTable[h] = next[o];
		next[o] = -1;
	    }
	    // delete obj
	    c_delete(table[obji]);	// call user-defined on-delete callback on: table[hashTable[obji]];
	    // replace hole by last pair
	    nbPairs--;
	    if(obji != nbPairs)
	    {
		Pair p = table[nbPairs];
		table[obji] = p;
		// update last pair's hash id
		int nh = hash(p.id1, p.id2) & mask;
		if(hashTable[nh]  == nbPairs) // last obj is in hash table
		    hashTable[nh] = obji;
		else
		{
		    // search in next[]
		    int no = hashTable[nh];
		    while(next[no] != nbPairs) // should never be -1
			no = next[no];
		    next[no] = obji;
		}
		next[obji] = next[nbPairs];
		next[nbPairs] =  -1;
	    }
	    // else OK : nothing to do
	    return true;
	}
	else
	    return false;
    }
}
