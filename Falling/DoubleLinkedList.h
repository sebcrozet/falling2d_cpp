#ifndef LIST
#include "stdafx.h"

template <typename T> 
class DoubleLinkedList
{
private:
	DoubleLinkedList<T> *next,*parent;
	T *value;
public:
	DoubleLinkedList(T *value);	
	DoubleLinkedList(T *value, DoubleLinkedList<T> *p, DoubleLinkedList<T> *n);

	void InsertAfter(DoubleLinkedList<T> *other);
	DoubleLinkedList<T> *InsertAfter(T *value);
	void InsertBefore(DoubleLinkedList<T> *other);
	DoubleLinkedList<T> *InsertBefore(T *value);

	void Remove();
	void DestroyList();
	void Remove(bool destroyValue);	 
	void DestroyList(bool destroyValue);

	void ForAll(void (*f) (T&));

	inline DoubleLinkedList<T> *getnext();
	inline DoubleLinkedList<T> *getprev();	
	inline void setnext(DoubleLinkedList<T> *n);
	inline void setparent(DoubleLinkedList<T> *p);
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
