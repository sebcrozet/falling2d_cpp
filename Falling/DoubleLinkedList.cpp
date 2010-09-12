#include "stdafx.h"
#include "DoubleLinkedList.h"

#pragma region Constructors
template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(T *e)
{
	next = 0;
	parent = 0;
	value = e;
}			
template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(T *e, DoubleLinkedList<T> *p, DoubleLinkedList<T> *n)
{
	next = n;
	parent = p;
	value = e;
	if(p)
		p->next = this;
	if(n)
		n->parent = this;
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
DoubleLinkedList<T> *DoubleLinkedList<T>::InsertAfter(T *e)
{	  
	DoubleLinkedList<T> *res = (new DoubleLinkedList<T>(e));
	InsertAfter(res);
	return res;
}
	 
template <typename T>
void DoubleLinkedList<T>::InsertBefore(DoubleLinkedList<T> *other)
{
	CircilarDoubleLinkedList<T> *p = parent;
	p->next = other;
	other->parent = p;
	other->next = this;
	parent = other;
}
	  
template <typename T>
DoubleLinkedList<T> * DoubleLinkedList<T>::InsertBefore(T *e)
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
void DoubleLinkedList<T>::Remove()
{
	next->parent = parent;
	parent->next = next;
	delete this;
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
void DoubleLinkedList<T>::ForAll(void (*f)(T&))
{
	DoubleLinkedList<T> *parc = this;
	do
	{
		f(parc->value);
		parc = parc->next;
	} while(parc!=this);
}