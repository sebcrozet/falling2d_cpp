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

	void Remove();
	void DestroyList();
	void Remove(bool destroyValue);	 
	static void RemoveExisting(DoubleLinkedList<T> *list, T val);
	void DestroyList(bool destroyValue);

	static DoubleLinkedList<T> *findExisting(DoubleLinkedList<T> *curr, T val)
	void ForAll(void (*f) (T&));

	inline DoubleLinkedList<T> *getnext();
	inline DoubleLinkedList<T> *getprev();	
	inline void setnext(DoubleLinkedList<T> *n);
	inline void setparent(DoubleLinkedList<T> *p);
	inline T getValue()
	{ return value; }
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
#define LIST
#endif
