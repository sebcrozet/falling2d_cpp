#ifndef LIST
#include "stdafx.h"

template <typename T> 
class CircularDoubleLinkedList
{
private:
	CircularDoubleLinkedList<T> *next,*parent;
	T *value;
public:
	CircularDoubleLinkedList(T *value);

	void InsertAfter(CircularDoubleLinkedList<T> *other);
	void InsertAfter(T *value);
	void InsertBefore(CircularDoubleLinkedList<T> *other);
	void InsertBefore(T *value);

	void Remove(bool destroyValue);	 
	void DestroyList(bool destroyValue);

	void ForAll(void (*f) (T));
};
#define LIST
#endif
