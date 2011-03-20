#include "stdafx.h"
#include "Tesselator.h"
//#pragma region Edge
/*
 * Constructor
 */
Edge::Edge(Poly *poly1, Poly *poly2) : p1(poly1), p2(poly2)
{ }
//#pragma endregion

//#pragma region Poly
/*
 * Constructor
 */
Poly::Poly() : parckey(0) ,parcstate(false)
{ }
/*
 * Destructor
 * Free both double linked lists but NOT thein contents
 * (Either or not botht edge's poly are null)
 */
Poly::~Poly()
{
    //adj->DestroyList();
    //pts->DestroyList();
}


//#pragma region As 
void Poly::testAdj()
{
    DoubleLinkedList<Edge *> *adjparc = this->adj;
    DoubleLinkedList<Point *> *parc = this->pts;
    // test if not degenerate
    if(this->pts->getnext() == this->pts->getprev())
	assert(false);
    // test if double point situation occurs in pseudo-polygon
    parc = this->pts;
    do
    {
	DoubleLinkedList<Point *> *parc2 = parc->getnext();
	do
	{
	    if(parc2->getValue() == parc->getValue())
		printf("NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOON il disait vrai!!!!");
	    parc2 = parc2->getnext();
	} while(parc2 != parc);
	parc = parc->getnext();
    } while(parc != this->pts);
    //
    do
    {
	Poly *o = adjparc->getValue()->getOther(this);
	if(o)
	{
	    DoubleLinkedList<Point *> *oparc = o->pts;
	    Point *l1 = 0, *l2 = 0;
	    while(!l1 || !l2)
	    {
		// will loop indefinitely here if test fails.
		if(oparc->getValue() == parc->getValue())
		    l1 = oparc->getValue();
		if(oparc->getValue() == parc->getprev()->getValue())
		    l2 = oparc->getValue();
		oparc = oparc->getnext();
	    }
	}
	adjparc = adjparc->getnext();
	parc = parc->getnext();
    } while(adjparc != this->adj);
    // test points adjascents
    parc = this->pts;
    do
    {
	// will loop indefinitely here if test fails.
	DoubleLinkedList<Poly *>::findExisting(parc->getValue()->adjlist,this);
	parc = parc->getnext();
    } while(parc != this->pts);
}

/*
 * removeFromGraphAndDelete()
 * Removes the actual triangle
 * from the polygon graph.
 * also free empty edges.
 * NOTE: delete this is automaticly called
 */
void Poly::removeFromGraphAndDelete()
{
    testAdj();
    pts->getValue()->removeAdjascent(this);
    pts->getnext()->getValue()->removeAdjascent(this);
    pts->getprev()->getValue()->removeAdjascent(this);
    adj->getValue()->replace(this, 0);
    adj->getprev()->getValue()->replace(this, 0);
    adj->getnext()->getValue()->replace(this, 0);
    if(adj->getValue()->isnull())
	delete adj->getValue();
    if(adj->getnext()->getValue()->isnull())
	delete adj->getnext()->getValue();
    if(adj->getprev()->getValue()->isnull())
	delete adj->getprev()->getValue();
    delete this;
}

/*
 * getOppositeEdgeAndAdjascent(...)
 * Finds the edge opposite to a given point in this triangle.
 * (i-e the edge which do not share the given point).
 * Also retrieves pointers to the two other points
 **
 * Prototype:
 * getOppositeEpgeAndAdjascent(
 *	Point *refpoint	: only point not shared with the opposit edge.
 *	Point **opppt1	: pointer which will be set to the rigth point of the opposite edge
 *	Point **opppt2	: pointer which will be set to the left point of the opposite edge
 *	)
 **
 * Return : 
 * Returns a pointer to the opposite edge
 */
Edge *Poly::getOppositeEdgeAndAdjascent(Point *refpoint, Point **opppt1, Point **opppt2)
{
    DoubleLinkedList<Point *> *curr = pts;
    DoubleLinkedList<Edge *> *curradj = adj;
    while(curr->getValue() != refpoint)
    {
	curr = curr->getnext();
	curradj = curradj->getnext();
    }
    *opppt1 = curr->getnext()->getValue();
    *opppt2 = curr->getprev()->getValue();
    return curradj->getprev()->getValue();
}

/*
 * getOppositePoint(...)
 * Find the pointer to the third point of this triangle.
 **
 * Prototype :
 * getOppositePoint(
 *	Point *r1	: first point not searched
 *	Point *r2	: second point not searched
 *	)
 *	r1 and r2 do not need to be ordoned
 **
 * Return :
 * Returns a pointer to the third point.
 */
Point *Poly::getOppositePoint(Point *r1, Point *r2)
{
    DoubleLinkedList<Point *> *curr = pts;
    while(curr->getValue() == r1 || curr->getValue() == r2)
	curr = curr->getnext();
    return curr->getValue();
}

/*
 * split(...)
 * Splits this thiangle in three parts and  updates
 * adjascency information.
 * It is suposed (but not needed) that the spliting
 * point do not falls on one of this triangle's edge.
 * The splitting point must fall in this triangle's
 * interior.
 **
 * Prototype :
 * split(
 *	Point *splitpt	: Splitting point
 *	Poly **oth1	: Pointer to be filled with one of the cutting-resulting triangle
 *	Poly **oth2	: Pointer to be filled with one of the cutting-resulting triangle
 *	)
 **
 * Returns void but set the results of the split operation on the given pointers.
 */
void Poly::split(Point *splitpt, Poly **oth1, Poly **oth2)
{
    // split in three triangles:
    // manipulate links
    DoubleLinkedList<Point *>
	*npt1, *npt2, *npt3,
	*prevptcpy, *nextptcpy, *thiscpy, *next;

    npt1 = new DoubleLinkedList<Point *>(splitpt);
    npt2 = new DoubleLinkedList<Point *>(splitpt);
    npt3 = new DoubleLinkedList<Point *>(splitpt);
    thiscpy = new DoubleLinkedList<Point *>(pts->getValue());
    prevptcpy = new DoubleLinkedList<Point *>(pts->getprev()->getValue());
    next = pts->getnext();
    nextptcpy = new DoubleLinkedList<Point *>(next->getValue());

    next->Unlink();
    pts->InsertAfter(npt1);
    splitpt->addAdjascent(this);
    // ok for polygon 1

    thiscpy->InsertAfter(nextptcpy);
    nextptcpy->InsertAfter(npt2);
    Poly *poly2 = new Poly();
    poly2->pts = thiscpy;
    thiscpy->getValue()->addAdjascent(poly2);
    nextptcpy->getValue()->addAdjascent(poly2);
    splitpt->addAdjascent(poly2);
    // ok for polygon 2

    next->InsertAfter(prevptcpy);
    prevptcpy->InsertAfter(npt3);
    Poly *poly3 = new Poly();
    poly3->pts = next;
    next->getValue()->addAdjascent(poly3);
    splitpt->addAdjascent(poly3);
    prevptcpy->getValue()->addAdjascent(poly3);
    // ok for polygon 3	   
    next->getValue()->removeAdjascent(this);

    // Now insert and update edges
    DoubleLinkedList<Edge *>
	*ne1, *ne2, *ne3,
	*ne1cpy, *ne2cpy, *ne3cpy,
	*preve, *nexte;
    ne1 = new DoubleLinkedList<Edge *>(new Edge(this,poly2));
    ne2 = new DoubleLinkedList<Edge *>(new Edge(poly3,poly2));
    ne3 = new DoubleLinkedList<Edge *>(new Edge(this,poly3));
    ne1cpy = new DoubleLinkedList<Edge *>(ne1->getValue());
    ne2cpy = new DoubleLinkedList<Edge *>(ne2->getValue());
    ne3cpy = new DoubleLinkedList<Edge *>(ne3->getValue());
    nexte = adj->getnext();
    preve = adj->getprev();

    nexte->Unlink();
    preve->Unlink();
    adj->InsertAfter(ne1);
    ne1->InsertAfter(ne3);
    // ok for poly 1
    poly2->adj = ne1cpy;
    ne1cpy->InsertAfter(nexte);
    nexte->InsertAfter(ne2);
    nexte->getValue()->replace(this, poly2);
    // ok for poly 2
    poly3->adj = ne2cpy;
    ne2cpy->InsertAfter(preve);
    preve->InsertAfter(ne3cpy);
    preve->getValue()->replace(this, poly3);
    // ok for poly 3
    *oth1 = poly2;
    *oth2 = poly3;
    poly2->testAdj();
    poly3->testAdj();
    this->testAdj();
}

/*
 * split(...)
 * Splits this triangle in two parts
 * and updates adjascency informations.
 * The given point is considered to
 * fall on the edge passed in parameter
 * (under the form of two points).
 **
 * Prototype :
 * split(
 *	Point *splitpt	: Splitting p—oint
 *	Point *pt1e	: Right point of the edge to be splitted
 *	Point *pt2e	: Left point of the edge to be splitted
 *	Poly **adjsplitted	: pointer which will be set to adjascent split
 *	  result.
 *	)
 * pt1e and pt2e must be chosen according to the CCW orientation of this triangle
 * (and so in CW order of the adjascent triangle).
 **
 * Return :
 * Return the second polygon result of the splitting process.
 */
Poly *Poly::split(Point *splitpt, Point *pt1e, Point *pt2e, Poly **adjsplitted)
{
    DoubleLinkedList<Point *> *l1 = 0, *thirdpt = 0, *parc = pts;
    DoubleLinkedList<Edge *> *ad2 = 0, *adjparc = adj;
    int i = 0;
    // search in this
    while(i < 3)
    {
	if(parc->getValue() == pt1e)
	    l1 = parc;
	else if(parc->getValue() == pt2e)
	    ad2 = adjparc;
	else
	    thirdpt = parc;
	parc = parc->getnext();
	adjparc = adjparc->getnext();
	i++;
    }
    // search poly 2
    Poly *pol2 = ad2->getValue()->getOther(this);
    // find edge and point list pointers corresponding to pt1 and pt2
    DoubleLinkedList<Point *> *l1o = 0, *thirdo = 0;
    DoubleLinkedList<Edge *> *ad2o = 0;
    parc = pol2->pts;
    adjparc = pol2->adj;
    while(i > 0)
    {
	if(parc->getValue() == pt2e)
	    l1o = parc;
	else if(parc->getValue() == pt1e)
	    ad2o = adjparc;
	else
	    thirdo = parc;
	parc = parc->getnext();
	adjparc = adjparc->getnext();
	i--;
    }
    //
    this->pts = thirdpt;
    this->adj = ad2->getnext();
    pol2->pts = thirdo;
    pol2->adj = ad2o->getnext();
    testAdj();
    pol2->testAdj();
    l1 = l1->InsertAfter(splitpt);
    l1o = l1o->InsertAfter(splitpt);
    splitpt->addAdjascent(this);
    splitpt->addAdjascent(pol2);
    ad2->InsertBefore(new Edge(ad2->getValue()->p1, ad2->getValue()->p2));
    ad2o->InsertAfter(ad2->getprev()->getValue());
    // TODO: remove test
    testAdj();
    pol2->testAdj();
    //printf("posttest");
    // end TODO
    // now these objects can be seen as a polygons ==> can use split function
    Poly *npoly = this->split(thirdpt->getValue(), l1->getValue());
    this->testAdj();
    npoly->testAdj();
    (*adjsplitted) = pol2->split(thirdo->getValue(),l1o->getValue());
    npoly->testAdj();
    this->testAdj();
    //printf("porstnpoly");
    (*adjsplitted)->testAdj();
    pol2->testAdj();
    return npoly;
}

/*
 * isInCircumcircle(...)
 * Determines if a given point is contained
 * by this triangle's circumcircle.
 **
 * Prototype :
 * isInCircumcircle(
 *	Point *pt	: Point to be tested
 *	)
 **
 * Returns true if the point is contained by the
 * circumcircle. Returns false otherwise.
 */
bool Poly::isInCircumcircle(Point *pt)
{
    return Point::isInCircumcircle(pts->getValue(), pts->getnext()->getValue(), pts->getprev()->getValue(), pt);
}

/*
 * testAndSwap(...)
 * Tests the Delauney's property of this triangle
 * determining if the third point of the opposed 
 * triangle does not lie on this triangle's
 * circumcircle.
 * If Delauney's property is no satisfied, the
 * edge shared by this and the opposite triangle
 * is swapped.
 **
 * Prototype :
 * testAnpSwap(
 *	Point *pt	: Point of this triangle not shared with the opposite triangle to be tested.
 )
 **
 * Return:
 * Returns a pointer to the other triangle after the swap process.
 * This point can be different than the original pointer (and the
 * original one is automaticly freed).
 * Returns 0 if no swap occured.
 */
Poly *Poly::testAndSwap(Point *pt)
{
    Point *p1, *p2, *po;
    Edge *opp = getOppositeEdgeAndAdjascent(pt, &p1, &p2);
    Poly *other = opp->getOther(this);
    if(!other)
	return 0;
    po = other->getOppositePoint(p1,p2);
    if(isInCircumcircle(po))
    {
	// swap middle edge
	// TODO: find a better way to do that?
	this->merge(other, p1, p2);
	return this->split(pt, po);
    }
    else return 0;
}
//#pragma endregion


//#pragma region As peudo-polygon
/*
 * split(...)
 * Splits this polygon in two parts.
 * CCW orientation of the resulting polygons
 * are conserved.
 **
 * Prototype :
 * split(
 *	Point *p1	: right point of the spliting edge
 *	Point *p2	: left point of the spliting edge
 *	)
 * p1 and p2 must be included in the polygon
 **
 * Return :
 * Return the second pseudo-polygon resulting from  the splitting process.
 */
Poly *Poly::split(Point *p1, Point *p2)
{
    // TODO: remove test
    if(pts->getnext()->getnext()->getnext() == pts)
	assert(false);
    if(pts->getnext()->getnext() == pts)
	assert(false);
    // end todo
    DoubleLinkedList<Point *> *l1 = 0, *l2 = 0, *parc = pts;
    DoubleLinkedList<Edge *> *ad1 = 0, *ad2 = 0, *adjparc = adj;
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
    Poly *res = new Poly();
    Edge *ed = new Edge(this,res);
    pts->Split(l1,l2);
    adj->Split(ad1,ad2,ed,ed);
    pts = l1;
    adj = ad1;
    res->adj = ad2;
    res->pts = l2;
    // update points adjascency
    adjparc = ad2;
    do
    {
	if(adjparc->getValue() != ed)
	    adjparc->getValue()->replace(this, res);
	adjparc = adjparc->getnext();
    } while(adjparc != ad2);
    parc = l2;
    do
    {
	if(parc->getValue() != p1 && parc->getValue() != p2)
	    parc->getValue()->replace(this, res);
	parc = parc->getnext();
    } while(parc != l2);  
    p1->addAdjascent(res);
    p2->addAdjascent(res);
    //this->testAdj();
    //res->testAdj();
    return res;
}


/*
 * merge(...)
 * Merges this polygon with another CCW-oriented polygon.
 * The merge is done "gluing" two common points on both polygons.
 **
 * Prototype :
 * merge(
 *	Poly *next	: polygon with which this polygon must be merged.
 *	  Next pointer is automaticly deleted.
 *	Point *p1	: right merging point.
 *	Point *p2	: left merging point.
 *	)
 * p1 and p2 must be givent in CCW order of this polygon (and so in CW order of 'next' polygon).
 */
void Poly::merge(Poly *next, Point *p1, Point *p2)
{
    mergeNoDelete(next, p1, p2);
    delete next;
}
void Poly::mergeNoDelete(Poly *next, Point *p1, Point  *p2)
{
    // search nodes in chain 1
    DoubleLinkedList<Point *> *l1 = 0, *l2 = 0, *parc = pts;
    DoubleLinkedList<Edge *> *ad1 = 0, *ad2 = 0, *adjparc = adj;
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
    // search nodes in chain 2
    DoubleLinkedList<Point *> *l1o = 0, *l2o = 0;
    DoubleLinkedList<Edge *> *ad1o = 0, *ad2o = 0;
    adjparc = next->adj;
    parc = next->pts;
    while(!l1o || !l2o)
    {
	if(parc->getValue() == p1)
	{
	    l1o = parc;
	    ad1o = adjparc;
	}
	else if(parc->getValue() == p2)
	{
	    l2o = parc;
	    ad2o = adjparc;
	}
	parc = parc->getnext();
	adjparc = adjparc->getnext();
    }	
    // modify adjascency in next's edges
    adjparc = ad2o;
    do
    {
	adjparc->getValue()->replace(next, this);
	adjparc = adjparc->getnext();
    } while(adjparc!=ad2o);
    // modify adjascency infos in next's points
    parc = l2o;
    do
    {	   
	if(parc->getValue() != p1 && parc->getValue()!= p2)
	    parc->getValue()->replace(next, this);
	parc = parc->getnext();
    } while(parc!=l2o);
    p1->removeAdjascent(next);
    p2->removeAdjascent(next);
    // merge both
    l1o = l1o->getnext();
    l2o = l2o->getprev();
    if(next->pts == l1o->getprev())
	next->pts = l1o;
    l1o->getprev()->Remove();
    if(next->pts == l2o->getprev())
	next->pts = l2o;
    l2o->getnext()->Remove();
    DoubleLinkedList<Point *>::Merge(l1,l2,l1o,l2o);
    ad2 = ad2->getnext();
    ad1o = ad1o->getnext();
    bool changehead = this->adj == ad1->getnext(); 
    if(changehead)
	this->adj = this->adj->getnext();
    ad1->getnext()->Remove();
    if(next->adj == ad2o->getnext())
	next->adj = ad2o;
    ad2o->getnext()->Remove(true); // delete the edge
    DoubleLinkedList<Edge *>::Merge(ad1,ad2,ad1o,ad2o);
    if(changehead)
	this->adj = this->adj->getprev();
    // TODO: remove test
    testAdj();
    this->testAdj();
}
//#pragma endregion
//#pragma endregion

//#pragma region Point
/*
 * Constructor.
 */
Point::Point() : adjlist(0), marked(false)
{ }

/*
 * Constructor
 */
Point::Point(const Point2D &p) : pt(p), adjlist(0), marked(false)
{ }

/*
 * Destructor
 */
Point::~Point()
{
    if(adjlist)
	adjlist->DestroyList();
}

/* * isInCircumcircle(...)
 * * Determine if a point is in the circumcircle of a given CCW triangle.
 * **
 * * Prototype :
 * * isInCircumcircle(
 * *	Point *pa	: first point of the CCW triangle
 * *	Point *pb	: second point of the CCW triangle
 * *	Point *pc	: third point of the CCW triangle
 *	Point *pt	: point to be tested
 *	*	)
 *	**
 *	* Return :
 *	* Returns true if pt is in the circumcircle, false otherwise.
 *	*/
bool Point::isInCircumcircle(Point *pa, Point *pb, Point *pc, Point *pt)
{
    Real ax =pa->pt.getX(),
	  ay = pa->pt.getY(),
	  bx =pb->pt.getX(),
	  by = pb->pt.getY(),
	  cx =pc->pt.getX(),
	  cy = pc->pt.getY(),
	  dx = pt->pt.getX(),
	  dy = pt->pt.getY();
    Real a = ax - dx,
	  b = ay - dy,
	  c = (ax*ax  - dx*dx) + (ay*ay - dy*dy),
	  d = bx - dx,
	  e = by - dy,
	  f = (bx*bx  - dx*dx) + (by*by - dy*dy),
	  g = cx - dx,
	  h = cy - dy,
	  i = (cx*cx  - dx*dx) + (cy*cy - dy*dy);
    return a*(e*i - f*h) - d*(b*i-c*h) + g*(b*f-e*c) > 0.;
}

/*
 * addAdjascent(...)
 * Add an adjascent polygon to this point
 **
 * Prototype
 * addAdjascent(
 *	Poly *toadd	: polygon to add
 *	)
 */
 void Point::addAdjascent(Poly *toadd)
 {
     if(!adjlist)
	 adjlist = new DoubleLinkedList<Poly *>(toadd);
     else
	 adjlist->InsertBefore(toadd);	
 }

 void Point::replace(Poly *p, Poly *np)
 {
     // p must exists in adj chain
     DoubleLinkedList<Poly *> *parc = this->adjlist;
     do
     {
	 if(parc->getValue() == p)
	 {
	     parc->setValue(np);
	     break;
	 }
	 parc = parc->getnext();
     }while(true);
}

/*
 * removeAdjascent(...)
 * Removes an adjascent polygon
 **
 * Prototype
 * removeAjascent(
 *	Poly *toremove	: polygon to remove
 *	)
 */
 void Point::removeAdjascent(Poly *toremove)
 {
     adjlist = DoubleLinkedList<Poly *>::RemoveExisting(adjlist, toremove);
 }
//#pragma endregion

//#pragma region Tesselations procedures
/*
 * triangleMarching(...)
 * Finds the triangle containing a given (end) point passing through every triangles
 * between (begin) point and (end) point.
 * Also merge triangles if asked to.
 **
 * Prototype :
 * triangleMarching(
 *	Point *begin	: beinning point of the path
 *	Point2D &end	: point to be found
 *	Point *graphpt	: pointer to a pointer which will receive the Point * corresponding to the current point
 *	)
 **
 * Return :
 * Returns a pointer to the polygon containing (end) point.
 * If end already existed, 0 is returned.
 */

Poly *Tesselator::triangleMarching(Point *begin, Point2D &end, Point **graphpt)
{
    if(begin->pt.errorEquals(end))
    {
	(*graphpt) = begin;
	return 0; // no path to follow
    }
    // find first triangle
    DoubleLinkedList<Poly *> *curr = begin->adjlist;
    Poly *poly;
    Poly *opppoly;
    Point *p1, *p2;
    printf("Beginning ...\n");
    do
    {
	poly = curr->getValue();
	opppoly = poly->getOppositeEdgeAndAdjascent(begin,&p1, &p2)->getOther(poly);
	// test if point included (any kind of inclusion, can be equal to a point) in the triangle
	if(end.isInCCWTriangle(begin->pt, p1->pt, p2->pt))
	    // stop march and return current triangle
	{
	    if(end.errorEquals(p1->pt)) 
	    {
		(*graphpt) = p1;
		return 0; // point already in graph
	    }
	    else if(end.errorEquals(p2->pt))
	    {
		(*graphpt) = p2;
		return 0; // point already in graph
	    }
	    else
	    {
		(*graphpt) = new Point(end);
		return poly;
	    }
	}
	Real l1 = p1->pt.isLeftTo(begin->pt, end); 
	Real l2 = p2->pt.isLeftTo(begin->pt, end);
	Real bl1 = p1->pt.isLeftTo(begin->pt, end); 
	Real bl2 = p2->pt.isLeftTo(begin->pt, end);
	Real dotl1 =Vector2D(begin->pt, end).dot(Vector2D(begin->pt,p1->pt));
	Real dotl2 =Vector2D(begin->pt, end).dot(Vector2D(begin->pt,p2->pt));
	if(p1->pt.isInLine(begin->pt, end) && dotl1 > 0.0)
	{
	    // point of triangle in the same line
	    printf("(1) Rec with: %f , % f \n\r", p1->pt.getX(), p1->pt.getY());
	    return triangleMarching(p1, end, graphpt);
	}
	else if(p2->pt.isInLine(begin->pt, end) && dotl2 > 0.0)
	{
	    // point of triangle in the same line
	    printf("(2) Rec with: %f , % f \n\r", p2->pt.getX(), p2->pt.getY());
	    return triangleMarching(p2, end, graphpt);
	}
	else if(l1 < 0.0 && l2 > 0.0)
	{
	    break;
	}
	printf("Refusing: %f ... %f || %f ... %f  \n", l1, l2, bl1, bl2);
	// else, not the good triangle
	curr = curr->getnext();
    } while(true);
    // if no valid triangle has been found... there is a problem!
    // So, assume the triangle is valid here
    // verify if point is not in this triangle
    // march on next triangle
    // (point is not in this triangle due to
    // previous tests).
    printf("Accepting...\n");
    poly = opppoly;	
    //printf("Not found!\n\r");
    while(true)
    {
	//printf("loop...");
	// swap p1 and p2 to be in CCW order
	Point *oldp1 = p1;
	p1 = p2;
	p2 = oldp1;
	Point *thirdpt = poly->getOppositePoint(p1,p2);
	Real lt = thirdpt->pt.isLeftTo(begin->pt, end); 
	// verify if we are not in the good triangle
	if(end.isInCCWTriangle(p1->pt,p2->pt,thirdpt->pt))
	{
	    // cannot be equal to p1 or p2
	    if(end.errorEquals(thirdpt->pt))
	    {
		(*graphpt) = thirdpt;
		return 0; // point already in graph
	    }
	    else
	    {
		(*graphpt) = new Point(end);
		return poly;
	    }
	}
	if(thirdpt->pt.isInLine(begin->pt, end))
	{
	    printf("(3) Rec with: %f , % f \n\r", thirdpt->pt.getX(), thirdpt->pt.getY());
	    return triangleMarching(thirdpt, end, graphpt);
	}
	else if(lt > 0.0)
	{	  
	    opppoly = poly->getOppositeEdgeAndAdjascent(p1, &p1, &p2)->getOther(poly);
	}
	else
	{
	    opppoly = poly->getOppositeEdgeAndAdjascent(p2, &p1, &p2)->getOther(poly);
	}
	poly = opppoly;
    }
}

/*
 * triangulate(...)
 * Triangulates this pseudo-polygon
 * recurcively, beginning with a triangle
 * having a specified base edge
 **
 * Prototype :
 * triangulate(
 *	Point *basep1	: left point of the base triangle of this CCW polygon.
 *	)
 * The second point of the base edge is automaticly determined as the point next to
 * basep1.
 */
void Poly::triangulate(Point *basep1)
{
    testAdj();
    if(pts->getnext()->getnext()->getnext() == pts)
	return; // cannot triangulate a triangle
    // find base points
    DoubleLinkedList<Point *> *r = 0, *l = 0, *curr = pts;
    while(!l)
    {
	if(curr->getValue() == basep1)
	{
	    l = curr;
	}
	curr = curr->getnext();
    };
    r = curr; // r = l->getnext();
    // find a good triangle
    curr = r->getnext();
    while(true)
    {
	// verify circumcircle rule
	// test left points
	DoubleLinkedList<Point *> *parc;
	parc = curr->getnext();
	bool notgood = false;
	while(parc != l)
	{

	    if(Point::isInCircumcircle(l->getValue(), r->getValue(), curr->getValue(), parc->getValue()))
	    {
		notgood = true;
		break;
	    }
	    parc = parc->getnext();
	}
	if(notgood)
	{
	    curr = curr->getnext();
	    continue; // not a valid candidate
	}
	// test right points
	parc = curr->getprev();
	while(parc != r)
	{	
	    if(Point::isInCircumcircle(l->getValue(), r->getValue(), curr->getValue(), parc->getValue()))
	    {
		notgood = true;
		break;
	    }
	    parc = parc->getprev();
	}
	if(!notgood)
	    break; // valid candidate found
	curr = curr->getnext();
    }
    // a valid candidate must have been found now
    // split current polygon in three parts and 
    // call recursion
    if(curr != r->getnext())
    {
	//printf("splitr\n");
	this->split(r->getValue(), curr->getValue())->triangulate(curr->getValue());
	this->testAdj();
    }
    // update curr pointer
    curr = r->getnext();
    if(curr!= l->getprev())
    {
	//printf("splitl\n");
	if(this->pts->getnext()->getnext()->getnext() == this->pts)
	    assert(false);
	this->split(curr->getValue(), l->getValue())->triangulate(l->getValue());
	this->testAdj();
    }
    // end of pseudo-polygon triangulation!
}
// Tesselator must be already initialized before running this
void Tesselator::insertPoint(Point2D &point, Point **gpoints)
{
    std::stack<Poly *> toVerify;
    Point *pt;
    // find triangle containing the point
    Poly *toSplit = Tesselator::triangleMarching(Tesselator::currpt, point, &pt);
    // if already exists, do nothing
    (*gpoints) = pt;
    if(!toSplit)
	return;
    // else determine split type
    // determine if point is in an edge
    DoubleLinkedList<Edge *> *curr = toSplit->adj;
    DoubleLinkedList<Point *> *currpt = toSplit->pts;
    int i;
    for(i = 0; i < 3; i++)
    {
	if(pt->pt.isInLine(currpt->getValue()->pt, currpt->getprev()->getValue()->pt))//pt->pt.isLeftTo(currpt->getValue()->pt, currpt->getprev()->getValue()->pt) == 0)
	    break;
	curr = curr->getnext();
	currpt = currpt->getnext();
    }
    Poly *oth2, *oth3;
    if(i == 3)
    {
	// not in an edge: split in three parts
	toSplit->split(pt, &oth2, &oth3);
	// push
	toVerify.push(toSplit);
    }
    else
    {
	// pt in in an edge: splic in 2*2 parts
	Poly *op = curr->getValue()->getOther(toSplit);
	oth2 = toSplit->split(pt, currpt->getprev()->getValue(), currpt->getValue(), &oth3);
	// push
	toVerify.push(toSplit);
	toVerify.push(op);
    }
    toVerify.push(oth2);
    toVerify.push(oth3);
    // Verify
    while(!toVerify.empty())
    {
	Poly *t = toVerify.top();
	toVerify.pop();
	Poly *topo = t->testAndSwap(pt);
	if(topo)
	{
	    //printf("SWAP!\n");
	    toVerify.push(t);
	    toVerify.push(topo);
	}
    }
    // set as last inserted point
    Tesselator::currpt = pt;
}
// Edge insertion
void Tesselator::insertEdge(Point *begin, Point *end)
{
    // find merged polygon
    if(begin == end)
	return ; // no path to follow
    // find first triangle
    DoubleLinkedList<Poly *> *curr = begin->adjlist;
    Poly *poly;
    Poly *opppoly;
    Point *p1, *p2;
    do
    {
	poly =  curr->getValue();

	opppoly = poly->getOppositeEdgeAndAdjascent(begin,&p1, &p2)->getOther(poly);
	if(end == p1 || end == p2)
	{
	    //printf("exists\n");
	    return;
	}

	Real l1 = p1->pt.isLeftTo(begin->pt, end->pt); 
	Real l2 = p2->pt.isLeftTo(begin->pt, end->pt);

	Real dotl1 = Vector2D(begin->pt, end->pt).dot(Vector2D(begin->pt,p1->pt));
	Real dotl2 = Vector2D(begin->pt, end->pt).dot(Vector2D(begin->pt,p2->pt));
	//printf("l1 -> %f ||", l1);
	//printf("l2 -> %f ||", l2);
	if(p1->pt.isInLine(begin->pt, end->pt) && dotl1 > 0.0)
	{
	    // point of triangle in the same line
	    if(p1->marked)
		printf("(0) Accepted a marked point: %p. Dot: %f\n", p1, dotl1);
	    else
		printf("Accepted a non-marked point: %f ... %f\n", p1->pt.getX(), p1->pt.getY());
	    insertEdge(p1, end);
	    return;
	}
	else if(p2->pt.isInLine(begin->pt, end->pt) && dotl2 > 0.0)
	{
	    // point of triangle in the same line
	    if(p2->marked)
		printf("(1) Accepted a marked point: %p. Dot: %f\n", p2, dotl2);
	    else
		printf("Accepted a non-marked point: %f ... %f\n", p2->pt.getX(), p2->pt.getY());
	    insertEdge(p2, end);
	    return;
	}
	else if(l1 < 0.0 && l2 > 0.0)
	{
	    break;
	}
	printf("Refusing %f ... %f \n", l1, l2);
	// else, not the good triangle
	curr = curr->getnext();
    } while(true);
    // march on next triangle
    // (point is not in this triangle due to
    // previous tests).	 
    Poly *tomerge = poly;	 
    poly = opppoly; 	  
    Point *oldp1, *oldp2;
    //printf("NOT\n");
    while(true)
    {	 
	oldp1 = p1;
	oldp2 = p2;
	// swap p1 and p2 to reset a CCW order
	p1 = p2;
	p2 = oldp1;

	Point *thirdpt = poly->getOppositePoint(p1,p2);	  
	if(end == thirdpt)
	    // cannot be equal to p1 or p2
	    break;

	Real lt = thirdpt->pt.isLeftTo(begin->pt, end->pt); 
	//printf("lt -> %f ||", lt);
	if(thirdpt->pt.isInLine(begin->pt, end->pt))
	{
	    if(thirdpt->marked)
		printf("(3) Accepted a marked point: %p.\n", thirdpt);
	    else
		printf("Accepted a non-marked point.");
	    insertEdge(thirdpt, end);
	    end = thirdpt;
	    break;
	}
	else if(lt > 0.0)
	{	  
	    //printf("merge > 0");
	    opppoly = poly->getOppositeEdgeAndAdjascent(p1, &p1, &p2)->getOther(poly);
	}
	else
	{
	    //printf("merge < 0");
	    opppoly = poly->getOppositeEdgeAndAdjascent(p2, &p1, &p2)->getOther(poly);
	}
	tomerge->merge(poly, oldp1, oldp2);
	// verify if we are not in the good triangle
	poly = opppoly;
    } 
    tomerge->merge(poly, oldp1, oldp2);
    // split and triangulate found polygon
    tomerge->split(begin, end)->triangulate(end);
    tomerge->triangulate(begin);
    // end of insertion
}
//#pragma endregion
//#pragma region Main function
void Tesselator::parcgraph(void(*print)(Poly *))
{	   
    // parc triangle graph (breadth first search)
    std::queue<Poly *> polys;
    polys.push(currpt->adjlist->getValue());
    int key = currpt->adjlist->getValue()->parckey + 1;
    currpt->adjlist->getValue()->parckey = key;
    while(!polys.empty())
    {
	Poly *pol = polys.front();
	polys.pop();
	// enqueue adjascent polygons
	DoubleLinkedList<Edge *> *curre = pol->adj;
	do
	{
	    Poly *pp = curre->getValue()->getOther(pol);
	    if(pp && pp->parckey != key)
	    {
		pp->parckey = key;
		polys.push(pp);
	    }
	    curre = curre->getnext();
	} while(curre != pol->adj);
	print(pol);
	// next
    }	 
}

void Tesselator::point2DListFromPointList(Poly *pol)
{
    // calculate size
    DoubleLinkedList<Point *> *curr = pol->pts;
    int size = 0;
    do
    {
	size++;
	curr = curr->getnext();
    } while(curr != pol->pts);
    (*subPolys)[ipoly] = new Point2D[size];
    (*nbptsSubPolys)[ipoly] = size;
    for(int i = 0; i < size; i++)
    {
	(*subPolys)[ipoly][i] = curr->getValue()->pt;
	curr = curr->getnext();
    }
}

int Tesselator::initAndRun(int removeMode, Point2D *pts, int nbpts, Point2D *holespts[], int nbholes, int nbptsholes[], Point2D ***respts, int **nbptsres)
{
    // find bounding box and insert
    // 4 virtual points (create a polygon
    // and split its diagonal)
    Real mx = MACHINE_MAX;
    Real Mx = -MACHINE_MAX;
    Real my = MACHINE_MAX;
    Real My = -MACHINE_MAX;
    for(int i = 0; i< nbpts; i++)
    {
	Real x= pts[i].getX();
	Real y = pts[i].getY();
	if(x < mx)
	    mx = x;
	if(x > Mx)
	    Mx = x;
	if(y < my)
	    my = y;
	if(y > My)
	    My = y;
    }
    for(int i = 0; i < nbholes; i++)
    {
	for (int j = 0; j < nbptsholes[i]; j++)
	{
	    Real x= holespts[i][j].getX();
	    Real y = holespts[i][j].getY();
	    if(x < mx)
		mx = x;
	    if(x > Mx)
		Mx = x;
	    if(y < my)
		my = y;
	    if(y > My)
		My = y;
	}
    }
    // scale the rectangle (to be sure no point will touch its external edges
    mx -= 10.0;
    my -= 10.0;
    Mx += 10.0;
    My += 10.0;
    // create fake rectangle
    // (allocate four edges and points)
    Poly *p = new Poly();
    Point *npt = new Point(Point2D(mx,my));
    npt->addAdjascent(p);
    p->pts = new DoubleLinkedList<Point *>(npt);
    npt = new Point(Point2D(Mx,my));
    npt->addAdjascent(p);
    p->pts->InsertAfter(npt);
    npt = new Point(Point2D(Mx,My));
    npt->addAdjascent(p);
    p->pts->getnext()->InsertAfter(npt);
    npt = new Point(Point2D(mx,My));
    npt->addAdjascent(p);
    p->pts->InsertBefore(npt);
    p->adj = new DoubleLinkedList<Edge *>(new Edge(p, 0));
    p->adj->InsertAfter(new Edge(p, 0));
    p->adj->getnext()->InsertAfter(new Edge(p, 0));
    p->adj->InsertBefore(new Edge(p, 0));

    // split into two triangles
    p->split(p->pts->getnext()->getValue(), p->pts->getprev()->getValue());

    // set prev point value as one of
    // the rectangle's points
    currpt = p->pts->getValue();
    // initialisation ended
    // loop on all pts and add them
    // add holes first (more performent if they are inside the polygon)
    // use lists to retain nodes
    Point *** ptgraphlist = new Point **[nbholes + 1];
    for(int i = 0; i < nbholes; i++)
    {
	(*ptgraphlist) = new Point *[nbptsholes[i]];
	for (int j = 0; j <	nbptsholes[i]; j++)
	    insertPoint(holespts[i][j], &(*(ptgraphlist + i)[j]));
    }
    (*(ptgraphlist + nbholes)) = new Point *[nbpts];
    for(int i = 0; i< nbpts; i++)
	insertPoint(pts[i], &((*(ptgraphlist + nbholes))[i]));
    // detect edges intersections and addn intersections points
    std::stack<Point *> steinerpoints;
    // TODO: remove test variable
    int nbinter = 0;
    // end todo
    for(int i = 0; i < nbholes; i++)
    {
	int k = nbptsholes[i] - 1;
	for (int j = 0; j <nbptsholes[i]; k = j, j++)
	{
	    // check intersections with other holes
	    for(int l = i + 1; l < nbholes; l++)
	    {
		int n = nbptsholes[l] - 1;
		for(int m = 0; m < nbptsholes[l]; n = m, m++)
		{
		    Real bparam1, bparam2;
		    Point2D inter;
		    bparam1 = Point2D::intersectSegments(holespts[i][k], holespts[i][j], holespts[l][n],holespts[l][m], &inter, &bparam2);
		    if(bparam1 != -1)
		    {
			nbinter++;
			Point *steiner;
			// insert intersecion point
			//printf("Intersection found: bparam: %f", bparam1);
			insertPoint(inter, &steiner);
			steinerpoints.push(steiner);
			steiner->marked = true;
		    }
		}
	    }
	    // check intersections with base polygon
	    int n = nbpts - 1;
	    for(int m = 0; m < nbpts; n = m, m++)
	    {
		Real bparam1, bparam2;
		Point2D inter;
		bparam1 = Point2D::intersectSegments(holespts[i][k], holespts[i][j], pts[n],pts[m], &inter, &bparam2);
		if(bparam1 != -1)
		{
		    nbinter++;
		    Point *steiner;
		    // insert intersecion point
		    //printf("Intersection found: bparam: %f", bparam1);
		    insertPoint(inter, &steiner);
		    steinerpoints.push(steiner);
		    steiner->marked = true;
		}
	    }
	    // check self-intersections
	    int p = j + 2;
	    for(int o = j + 3; o < nbptsholes[i] - 1; p = o, o++)
	    {
		Real bparam1, bparam2;
		Point2D inter;
		bparam1 = Point2D::intersectSegments(holespts[i][k], holespts[i][j], holespts[i][p],holespts[i][o], &inter, &bparam2);
		if(bparam1 != -1)
		{
		    nbinter++;
		    Point *steiner;
		    //printf("Intersection found: bparam: %f", bparam1);
		    // insert intersecion point
		    insertPoint(inter, &steiner);
		    steinerpoints.push(steiner);
		    steiner->marked = true;
		}
	    }
	}
    }
    int ptsm1 = nbpts - 1;
    for(int i = 0; i< nbpts - 1; ptsm1 = i, i++)
    {
	// check self-intersections
	int p = i + 1;
	for(int o = i + 2; o < (i == 0 ? nbpts - 1 : nbpts); p = o, o++)
	{
	    Real bparam1, bparam2;
	    Point2D inter;
	    bparam1 = Point2D::intersectSegments(pts[ptsm1], pts[i], pts[p],pts[o], &inter, &bparam2);
	    if(bparam1 != -1.0 && bparam1 != 0.0 && bparam2 != 0.0 && bparam1!= 1.0 && bparam2!=1.0)
	    {
		nbinter++;
		Point *useless;
		// insert intersecion point
		printf("Intersection found: bparam: %f\n", bparam1);
		printf("Intersection found: bparam2: %f\n", bparam2);
		if((!inter.isInLine(pts[ptsm1],pts[i])) || (!inter.isInLine(pts[p],pts[o])))
		{
		    printf("CORRUPTED INTERSECTION\n");
		    //Sleep(5000);
		    /*
		       while(true)
		       {
		       inter.isInLine(pts[ptsm1],pts[i]);
		       inter.isInLine(pts[p],pts[o]);
		       }*/
		}
		insertPoint(inter, &useless);
		printf("Intersection found: addr.: %p\n", useless);
		useless->marked = true;
	    }
	}
    }
    //
    // loop on all edges and add them
    for(int i = 0; i < nbholes; i++)
    {
	int k = nbptsholes[i] - 1;
	for (int j = 0; j <	nbptsholes[i]; k = j, j++)
	    insertEdge(ptgraphlist[i][k],ptgraphlist[i][j]);
    }
    int k = nbpts - 1;
    int i;
    for(i = 0; i< nbpts; k = i, i++)
    {
	printf("Edge inserting... \n");
	insertEdge(ptgraphlist[nbholes][k], ptgraphlist[nbholes][i]);	 
	printf("Edge inserted. \n");
    }
    // END of Constrained Delaunay Triangulation

    // Remove non-needed triangles
    // notice that every Poly struct
    // represents a triangle at this
    // point.
    std::stack<Poly *> remainingtriangles;
    removeExternalTriangles(removeMode, pts, nbpts, holespts, nbholes, nbptsholes, remainingtriangles);

    // batch triangles in InplicitPolygons2D
    // to construct the convex decomposition.
    subPolys = respts;
    nbptsSubPolys = nbptsres;
    std::stack<Poly *>respolys;
    int totpolys = batchTriangles(remainingtriangles, respolys);
    (*subPolys) = new Point2D *[totpolys];
    (*nbptsSubPolys) = new int[totpolys];
    //printf("Nbr: %i", totpolys);
    ipoly = 0;
    while(!respolys.empty())
    {
	point2DListFromPointList(respolys.top());
	respolys.pop();
	ipoly++;
    }
    // TODO: remove debug print
    //printf("INTER: %i\n", nbinter);
    //printf("ipoly: %i\n", ipoly);
    // end todo
    // tesselation ended!

    // free memory
    //for(int i = 0; i < nbholes; i++)
    //	delete[] (ptgraphlist[i]);
    //delete [](*(ptgraphlist + nbholes));
    //delete[] ptgraphlist;  

    // free graph memor
    // TODO (recurcive?)
    return totpolys;
}
    /*
     * removeExternalTriangles(...)
     * Remove triangles not needed for the polygonal triangulation.
     * Fusion type (when many polygonst are specified) is determined by the user.
     **
     * Prototype :
     * removeExternalTriangles(
     * int removeMode: fusion type code
     *	1 -> odd is solid
     *	2 -> even is solid
     *	4 -> all addition
     *	8 -> all substaction
     *	16 -> intersection
     *	32 -> all complementary intersection
     *	Point2D *pts	: points of the base polygon
     *	int nbpts	: number of points in the base polygon
     *	Point2D *holespots[]	: array of points of polygonal "holes"
     *	int nbholes	: number of holes in "holespots" array
     *	int nbptsholes[]	: array of number of points in each holes
     * Note: Points included in no polygon (neither in holes nore in base)
     * are always deleted (no matter which fusion type is chosen).
     * Note2: virtual points (added during delauney initialisation)
     * will be automatically deleted here.
     */
void Tesselator::removeExternalTriangles(
	int removeMode, 
	Point2D *pts, 
	int nbpts, 
	Point2D *holespts[], 
	int nbholes, 
	int nbptsholes[],
	std::stack<Poly *> &remaining) 
{
    // parc triangle graph (breadth first search)
    std::queue<Poly *> polys;
    polys.push(currpt->adjlist->getValue());
    currpt->adjlist->getValue()->parcstate = true;
    while(!polys.empty())
    {
	Poly *pol = polys.front();
	polys.pop();
	// enqueue adjascent polygons
	DoubleLinkedList<Edge *> *curre = pol->adj;
	for(int i = 0; i < 3; i++)
	{
	    Poly *pp = curre->getValue()->getOther(pol);
	    if(pp && !pp->parcstate)
	    {
		pp->parcstate = true;
		polys.push(pp);
	    }
	    curre = curre->getnext();
	}
	// handle current triangle
	// calculate the triangle's centroid
	Point2D centroid = 
	    Point2D::getMiddle(
		    pol->pts->getprev()->getValue()->pt,
		    pol->pts->getnext()->getValue()->pt);
	centroid =  pol->pts->getValue()->pt + (centroid - pol->pts->getValue()->pt)*(2./3.);
	// determine degree of inclusion of the point
	int inclusiondegree = 0;
	bool includedinbase = Point2D::pointInPolygon(centroid,pts,nbpts);
	if(!includedinbase)
	{
	    if(removeMode & 24)
	    {
		// remove this triangle
		pol->removeFromGraphAndDelete();
		continue;
	    }
	}
	else
	{
	    if(removeMode & 4)
	    {
		remaining.push(pol);
		continue;
	    }
	    inclusiondegree = 1;
	}
	bool breakcode = false;
	for(int i = 0; !breakcode && i < nbholes; i++)
	{
	    	centroid = 
		Point2D::getMiddle(
			Point2D::getMiddle(pol->pts->getValue()->pt, pol->pts->getnext()->getValue()->pt),
			pol->pts->getprev()->getValue()->pt);
	    if(Point2D::pointInPolygon(centroid,holespts[i],nbptsholes[i]))
	    {
		if(removeMode & 4)
		{
		    // do not remove
		    breakcode = true;
		    remaining.push(pol);
		}
		else if (removeMode & 8)
		{
		    // remove it
		    pol->removeFromGraphAndDelete();
		    breakcode = true;
		}
		inclusiondegree++;
	    }
	    else
	    {
		if(removeMode & (16))
		{
		    // remove it
		    pol->removeFromGraphAndDelete();
		    breakcode = true;
		}
		else if(removeMode & 32)
		{
		    // do not remove
		    breakcode = true;
		    remaining.push(pol);
		}
	    }
	}
	if(breakcode)
	    continue;
	if(inclusiondegree == 0)
	    pol->removeFromGraphAndDelete();
	else if((inclusiondegree & 1))
	{
	    // odd
	    if((removeMode & 2) || ((removeMode & 32) && !includedinbase))
		pol->removeFromGraphAndDelete();
	    else
		remaining.push(pol);
	}
	else
	{
	    // even
	    if((removeMode & 1))
		pol->removeFromGraphAndDelete();
	    else
		remaining.push(pol);
	}
	// next
    }
    // END: of bad-triangles deletion
}

// returns the final number of polygons
int Tesselator::batchTriangles(std::stack<Poly *> &toparcpol, std::stack<Poly *> &respolys)
{	    
    // parc triangle graph (breadth first search)
    int nbrres = 0;
    std::queue<Poly *> polys;
    std::stack<Poly *>toDelete;
    while(!toparcpol.empty())
    {
	Poly *headpoly = toparcpol.top();
	toparcpol.pop();
	if(!headpoly->parcstate)
	    continue;
	polys.push(headpoly);
	headpoly->parcstate = false;
	while(!polys.empty())
	{
	    Poly *pol = polys.front();
	    //printf("pop!  \n");
	    polys.pop();
	    respolys.push(pol);
	    nbrres++;
	    // enqueue adjascent polygons
	    bool retry;
	    do
	    {	   
		retry = false;
		DoubleLinkedList<Edge *> *curre = pol->adj;
		DoubleLinkedList<Point *> *currpt = pol->pts->getprev();
		do
		{
		    Poly *pp = curre->getValue()->getOther(pol);
		    if(pp && pp->parcstate)
		    {
			// find link in poly 2
			DoubleLinkedList<Point *> *opt = pp->pts;
			while(opt->getValue() != currpt->getValue())
			    opt = opt->getnext();
			// see if merge with triangle is possible. Make only proper polygons
			if(currpt->getValue()->pt.isLeftTo(currpt->getprev()->getValue()->pt, opt->getnext()->getValue()->pt) <  0.0
				&&
				currpt->getnext()->getValue()->pt.isLeftTo(opt->getprev()->getprev()->getValue()->pt,
				    currpt->getnext()->getnext()->getValue()->pt) < 0.0) 
			{
			    // ok, merge and retry
			    pol->mergeNoDelete(pp, currpt->getValue(), currpt->getnext()->getValue());
			    // mark as deletable polygon
			    pp->parcstate = false;
			    toDelete.push(pp);
			    pol->testAdj();
			    retry = true;
			    break;
			}	  
			//printf("compushed\n");
			pp->parcstate = false;
			polys.push(pp);
		    }
		    curre = curre->getnext();
		    currpt = currpt->getnext();
		} while(curre != pol->adj);
	    } while(retry);
	}
    }
    // delete polygons
    while(!toDelete.empty())
    {
	delete toDelete.top();
	toDelete.pop();
    }
    return nbrres;
}
//#pragma endregion
