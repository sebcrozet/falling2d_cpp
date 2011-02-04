#include "stdafx.h"
#include "DoubleLinkedList.h"

#pragma region Constructors
template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(T e)
{
	next = 0;
	parent = 0;
	value = e;
}			
template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(T e, DoubleLinkedList<T> *p, DoubleLinkedList<T> *n)
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
	DoubleLinkedList<T> *n1, *n2, *s2n, *spval1, *spval2;

	s2n = splitnode2->next;
	n1 = new DoubleLinkedList<T>(splitnode->value, splitnode2, splitnode->next);
	n2 = new DoubleLinkedList<T>(splitnode2->value, splitnode, s2n);
	spval1 = new DoubleLinkedList<T>(v1, splitnode, n2);
	spval2 = new DoubleLinkedList<T>(v2, splitnode2, n1);
	// splits are done at creation so its ok
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
	CircilarDoubleLinkedList<T> *p = parent;
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
void DoubleLinkedList<T>::Remove()
{
	next->parent = parent;
	parent->next = next;
	delete this;
}

template <typename T>
void DoubleLinkedList<T>::RemoveExisting(DoubleLinkedList<T> list, T val)
{
	findExisting(list,val)->Remove();
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