#ifndef _PAIRMANAGER_H
#include <vector>
typedef unsigned short ushort;	 
typedef unsigned int uint;
 
#define HASH_CHARGE_FACTOR 0.8f
struct Pair
{
	ushort id1;
	ushort id2;
	void *e;

	Pair(ushort i1 = -1, ushort i2 = -1);
	inline bool equals(ushort i1, ushort i2);
};

class PairManager
{
private:
	Pair *table;
	uint mask;
	int *hashTable;
	int *next;
	int nbPairs;
	int maxPairs, realmaxPairs;
	void (*c_delete) (Pair &p);

	inline void orderIDs(ushort &id1, ushort &id2);
	inline unsigned int buildid(ushort id1, ushort id2);
	inline uint hash(ushort id1, ushort id2);				
	inline uint hash(uint id);

	Pair *findPair(ushort id1, ushort id2, uint h);
	static void nop(Pair &p);
public:				   
	PairManager(void (*f)(Pair &p) = 0);
	~PairManager();

	Pair *findPair(ushort id1, ushort id2);
	Pair *addPair(ushort id1, ushort id2);
	bool removePair(ushort id1, ushort id2);
	bool removePair(Pair *p);

	inline Pair *getActivePairs();
	inline int getNbActivePairs();
	inline void setDeletecallback(void (*c_deletecall) (Pair &p));
};

inline bool Pair::equals(ushort i1, ushort i2)
{ return i1 == id1 && i2 == id2; } 

inline Pair *PairManager::getActivePairs()
{ return table; }

inline int PairManager::getNbActivePairs()
{ return nbPairs; }

inline void PairManager::orderIDs(ushort &id1, ushort &id2)
{ 
	if(id1 < id2)
	{
		ushort e = id2;
		id2 = id1;
		id1 = e;
	}
}   

inline uint PairManager::hash(ushort id1, ushort id2)
{ return hash(ushort(id1 & 0xffff) | ushort(id2 << 16)); }	

inline uint PairManager::hash(uint id)
{
	// Thomas Wang's hash
	// http://www.concentric.net/~Ttwang/tech/inthash.htm
	id = ~id + (id << 15);
	id ^= id >> 12;
	id += id << 2;
	id ^= id >> 4;
	id *= 2057;
	id ^= id >> 16;
	return id;
}   

inline void PairManager::setDeletecallback(void (*c_deletecall) (Pair &p))
{ c_delete = c_deletecall; }
#define _PAIRMANAGER_H
#endif



