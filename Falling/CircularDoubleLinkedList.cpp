#include "stdafx.h"
#include "CircularDoubleLinkedList.h"

#pragma region Constructor
template <typename T>
CircularDoubleLinkedList<T>::CircularDoubleLinkedList(T *e)
{
	next = this;
	parent = this;
	value = e;
}
#pragma endregion

#pragma region Insert
template <typename T>
void CircularDoubleLinkedList<T>::InsertAfter(CircularDoubleLinkedList<T> *other)
{
	 CircularDoubleLinkedList<T> *n = next;
	 n->parent = other;
	 other->next = n;
	 other->parent = this;
	 next = other;
}
   
template <typename T>
void CircularDoubleLinkedList<T>::InsertAfter(T *e)
{
	InsertAfter(new CircularDoubleLinkedList<T>(e));
}
	 
template <typename T>
void CircularDoubleLinkedList<T>::InsertBefore(CircularDoubleLinkedList<T> *other)
{
	CircilarDoubleLinkedList<T> *p = parent;
	p->next = other;
	other->parent = p;
	other->next = this;
	parent = other;
}
	  
template <typename T>
void CircularDoubleLinkedList<T>::InsertBefore(T *e)
{
	InsertBefore(new CircularDoubleLinkedList<T>(e));
}
#pragma endregion

#pragma region Remove
template <typename T>
void CircularDoubleLinkedList<T>::Remove(bool destroyValue)
{
	next->parent = parent;
	parent->next = next;
	if (destroyValue)
		delete value;
	delete this;
}
						  
template <typename T>
void CircularDoubleLinkedList<T>::DestroyList(bool destroyValue)
{
	while(parent!=next)
		next->Remove(destroyValue);
	delete this;
}
#pragma endregion

template <typename T>
void CircularDoubleLinkedList<T>::ForAll(void (*f)(T))
{
	CircularDoubleLinkedList<T> *parc = this;
	do
	{
		f(parc->value);
		parc = parc->next;
	} while(parc!=this);
}