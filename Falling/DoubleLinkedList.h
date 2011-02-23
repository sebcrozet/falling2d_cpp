#ifndef LIST
#include "stdafx.h"

template <typename T> 
class DoubleLinkedList
{
private:
	DoubleLinkedList<T> *next,*parent;
	T value;
public:
	DoubleLinkedList(T value);	
	DoubleLinkedList(T value, DoubleLinkedList<T> *p, DoubleLinkedList<T> *n);

	void InsertAfter(DoubleLinkedList<T> *other);
	DoubleLinkedList<T> *InsertAfter(T value);
	void InsertBefore(DoubleLinkedList<T> *other);
	DoubleLinkedList<T> *InsertBefore(T value);
	void Split(DoubleLinkedList<T> *splitnode, DoubleLinkedList<T> *splitnode2);
	void Split(DoubleLinkedList<T> *splitnode, DoubleLinkedList<T> *splitnode2, T lvalue, T rvalue);
	static void Merge(
		DoubleLinkedList<T> *mergnodeList1, 
		DoubleLinkedList<T> *mergenode2List1,
		DoubleLinkedList<T> *mergenodeList2,
		DoubleLinkedList<T> *mergenode2List2
		);



	void Remove();
	void DestroyList();
	void Unlink();
	void Remove(bool destroyValue);	 
	static DoubleLinkedList<T> *RemoveExisting(DoubleLinkedList<T> *list, T val);
	void DestroyList(bool destroyValue);

	static DoubleLinkedList<T> *findExisting(DoubleLinkedList<T> *curr, T val);
	void ForAll(void (*f) (T&));

	inline DoubleLinkedList<T> *getnext();
	inline DoubleLinkedList<T> *getprev();	
	inline void setnext(DoubleLinkedList<T> *n);
	inline void setparent(DoubleLinkedList<T> *p);
	inline T getValue()
	{ return value; }
	inline void setValue(T nv)
	{ value = nv; }
};
template <typename T> 
inline DoubleLinkedList<T> *DoubleLinkedList<T>::getnext()
{ return next; }   
template <typename T> 
inline DoubleLinkedList<T> *DoubleLinkedList<T>::getprev()
{ return parent; }   
template <typename T> 
inline void DoubleLinkedList<T>::setnext(DoubleLinkedList<T> *n)
{ next = n;	}	 
template <typename T>  
inline void DoubleLinkedList<T>::setparent(DoubleLinkedList<T> *p)
{ parent = p;	}

#pragma region Constructors
template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(T e)
{
	next = this;
	parent = this;
	value = e;
}			
template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(T e, DoubleLinkedList<T> *p, DoubleLinkedList<T> *n)
{
	next = n;
	parent = p;
	value = e;
	p->next = this;
	n->parent = this;
}
#pragma endregion

#pragma region Split
/* Splits:
   a->b->c->d->e->f->(a)
   To (split at c -> f):
   a->b->c->f'->(a) | c'->d->e->f->(c')
   c and f are links to the two new lists.
*/
template <typename T>
void DoubleLinkedList<T>::Split(DoubleLinkedList<T> *splitnode, DoubleLinkedList<T> *splitnode2)
{
	DoubleLinkedList<T> *n1, *n2, *s2n;
	s2n = splitnode2->next;
	n1 = new DoubleLinkedList<T>(splitnode->value, splitnode2, splitnode->next);
	n2 = new DoubleLinkedList<T>(splitnode2->value, splitnode, s2n);
	// splits are done at creation so its ok
}
/* Splits:
   a->b->c->d->e->f->(a)
   To (split at c -> f with v1 & v2):
   a->b->c->node(v1)->f'->(a) | c'->d->e->f->node(v2)->(c')
   c and f are links to the two new lists.
*/
template <typename T>
void DoubleLinkedList<T>::Split(DoubleLinkedList<T> *splitnode, DoubleLinkedList<T> *splitnode2, T v1, T v2)
{
	DoubleLinkedList<T> *s1n, *s2n, *spval1, *spval2;

	s1n = splitnode->next;
	s2n = splitnode2->next;
	spval1 = new DoubleLinkedList<T>(v1, splitnode, s2n);
	spval2 = new DoubleLinkedList<T>(v2, splitnode2, s1n);
	// splits are done at creation so its ok
}

/*
 * merges two lists:
 * a->b->c->(a) | d->e->f->(d)
 * merging b-f and e-c:
 * a->b->f->d->e->c->(a)
 */
template <typename T>
void DoubleLinkedList<T>::Merge(
		DoubleLinkedList<T> *mergenodeList1, 
		DoubleLinkedList<T> *mergenode2List1,
		DoubleLinkedList<T> *mergenodeList2,
		DoubleLinkedList<T> *mergenode2List2
		)
{
	mergenodeList1->next = mergenodeList2;
	mergenodeList2->parent = mergenodeList1;
	mergenode2List2->next = mergenode2List1;
	mergenode2List1->parent = mergenode2List2;
}
#pragma endregion

#pragma region Insert
template <typename T>
void DoubleLinkedList<T>::InsertAfter(DoubleLinkedList<T> *other)
{
	 DoubleLinkedList<T> *n = next;
	 n->parent = other;
	 other->next = n;
	 other->parent = this;
	 next = other;
}
   
template <typename T>
DoubleLinkedList<T> *DoubleLinkedList<T>::InsertAfter(T e)
{	  
	DoubleLinkedList<T> *res = (new DoubleLinkedList<T>(e));
	InsertAfter(res);
	return res;
}
	 
template <typename T>
void DoubleLinkedList<T>::InsertBefore(DoubleLinkedList<T> *other)
{
	DoubleLinkedList<T> *p = parent;
	p->next = other;
	other->parent = p;
	other->next = this;
	parent = other;
}
template <typename T>
DoubleLinkedList<T> * DoubleLinkedList<T>::InsertBefore(T e)
{
	DoubleLinkedList<T> *res = (new DoubleLinkedList<T>(e));
	InsertBefore(res);
	return res;
}
#pragma endregion

#pragma region Remove
template <typename T>
void DoubleLinkedList<T>::Remove(bool destroyValue)
{
	next->parent = parent;
	parent->next = next;
	if (destroyValue)
		delete value;
	delete this;
}

template <typename T>
void DoubleLinkedList<T>::Unlink()
{
	next->parent = parent;
	parent->next = next;
	parent = this;
	next = this;
}

template <typename T>
void DoubleLinkedList<T>::Remove()
{
	next->parent = parent;
	parent->next = next;
	delete this;
}

template <typename T>
DoubleLinkedList<T> *DoubleLinkedList<T>::RemoveExisting(DoubleLinkedList<T> *list, T val)
{									 
	DoubleLinkedList<T> *n = list->next;
	if(list == n)
	{
		delete list;
		return 0;
	}
	DoubleLinkedList<T> *todel = findExisting(list,val);
	todel->Remove();
	if(todel == list)
		return n; // head has been deleted
	else
		return list;
}

template <typename T>
void DoubleLinkedList<T>::DestroyList(bool destroyValue)
{
	while(parent!=next)
		next->Remove(destroyValue);
	delete this;
}
template <typename T>
void DoubleLinkedList<T>::DestroyList()
{
	while(parent!=next)
		next->Remove();
	delete this;
}
#pragma endregion

template <typename T>
DoubleLinkedList<T> *DoubleLinkedList<T>::findExisting(DoubleLinkedList<T> *curr, T val)
{
	// assumes the element already exists
	while(curr->value != val)
		curr = curr->next;
	return curr;
}
template <typename T>
void DoubleLinkedList<T>::ForAll(void (*f)(T&))
{
	DoubleLinkedList<T> *parc = this;
	do
	{
		f(parc->value);
		parc = parc->next;
	} while(parc!=this);
}

#define LIST
#endif
