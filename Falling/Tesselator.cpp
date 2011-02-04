#include "stdafx.h"
#include "Tesselator.h"

#pragma region Poly
#pragma region As triangle
Tesselator::Poly *Tesselator::Poly::getOppositeEdgeAndAdjascent(Tesselator::Point *refpoint, Tesselator::Point **opppt1, Tesselator::Point **opppt2)
{
	DoubleLinkedList<Tesselator::Point *> *curr = pts;
	DoubleLinkedList<Tesselator::Poly *> *curradj = adj;
	while(curr->getValue() != refpoint)
	{
		curr = curr->getnext();
		adj = adj->getnext();
	}
	*opppt1 = curr->getprev()->getValue();
	*opppt2 = curr->getnext()->getValue();
	return adj->getprev()->getValue();
}
Tesselator::Point *Tesselator::Poly::getOppositePoint(Tesselator::Point *r1, Tesselator::Point *r2)
{
	DoubleLinkedList<Tesselator::Point *> *curr = pts;
	while(curr->getValue() == r1 || curr->getValue() == r2)
		curr = curr->getnext();
	return curr->getValue();
		
}
void Tesselator::Poly::split(DoubleLinkedList<Tesselator::Point *> *splitpt)
{
}
#pragma endregion
#pragma region As peudo-polygon
// p1 and p2 must be included in the polygon
Tesselator::Poly *Tesselator::Poly::split(Point *p1, Point *p2)
{
	DoubleLinkedList<Tesselator::Point *> *l1 = 0, *l2 = 0, *parc = pts;
	DoubleLinkedList<Tesselator::Poly *> *ad1, *ad2, *adjparc = adj;
	while(!l1 || !l2)
	{
		if(parc->getValue() == p1)
		{
			l1 = parc;
			ad1 = adjparc;
		}
		else if(parc->getValue() == p2)
		{
			l2 = parc;
			ad2 = adjparc;
		}
		parc = parc->getnext();
		adjparc = adjparc->getnext();
	}
	Tesselator::Poly *res = new Tesselator::Poly();
	pts->Split(l1,l2);
	adj->Split(ad1,ad2,res,this);
	pts = l1;
	adj = ad1;
	res->adj = ad2;
	res->pts = l2;
	// update points adjascency
	p1->addAdjascent(res);
	p2->addAdjascent(res);
	return res;
}

//void Tesselator::Poly::merge(Tesselator::Poly *next,
#pragma endregion
#pragma endregion

#pragma region Point
void Tesselator::Point::addAdjascent(Poly *toadd)
{
	adjlist->InsertBefore(toadd);	
}

void Tesselator::Point::removeAdjascent(Poly *toremove)
{
	DoubleLinkedList<Tesselator::Poly *>::RemoveExisting(adjlist, toremove);
}
#pragma endregion