#include "stdafx.h"
#include "Polygon2D.h"
#include "GeometryHelper.h"
#include <vector>

#pragma region Polygon2D
Polygon2D::Polygon2D(Point2D * p,GeometryHelper::Transformation2D &tr,int n, bool fixed) : nbrSubShapes(0), ixm(0), ixM(0), iym(0), iyM(0)
{ 
	fixedobj = fixed;
	t = GeometryHelper::Transformation2D(Vector2D(),0);
	int nchull;
	Point2D *chullpts = 0;
	nbrPts = simplify(p, n, &points, 25);
	// get orientation and invert points if non CCW
	if(!isCCW())
	{
		for(int i = 0;i<nbrPts / 2;i++)
		{
			// swap
			Point2D tmp = points[i];
			points[i] = points[nbrPts - i - 1];
			points[nbrPts - i - 1] = tmp;
		}
	}
	nchull = Polygon2D::buildConvexHull(points, nbrPts, &chullpts);	 
	chull = new ImplicitPolygon2D(chullpts, nchull, this, 0);
	if(nchull != nbrPts) // le polygone n'était pas déjà convexe, il faut faire une tesselation
		tesselate();
	buildOBBtree();
	t.setTeta(tr.getTeta());
	t.setU(tr.getU());
}

bool Polygon2D::isCCW()
{
	// find lowest point
	int idp = 0;
	Point2D lpt = points[idp];
	for(int i = 1; i < nbrPts; i++)
	{
		Point2D tmp = points[i];
		if((lpt.getY() > tmp.getY()) ||(lpt.getY() == tmp.getY() && lpt.getX() < tmp.getY()))
		{
			idp = i;
			lpt = tmp;
		}
		// else next
	}
	// return cross product sign
	return points[(idp - 1 < 0)?nbrPts - 1:idp - 1].isLeftTo(lpt,points[(idp + 1 == nbrPts)?0:idp + 1]) > 0; 
}

void Polygon2D::updateAABB()
{
	Vector2D vx(1,0);
	Vector2D vy(0,1);
	Point2D pt;
	float m = chull->getMargin();
	ixM = chull->getSupportPoint(vx,&pt,ixM);
	aabb_xM = pt.getX() + m;
	ixm = chull->getSupportPoint(vx.reflexion(),&pt,ixm);
	aabb_xm = pt.getX() - m;
	iyM = chull->getSupportPoint(vy,&pt,iyM);
	aabb_yM = pt.getY() + m;
	iym = chull->getSupportPoint(vy.reflexion(),&pt,iym);
	aabb_ym = pt.getY() - m;
}

void Polygon2D::tesselate()
{
	std::vector<int> ipts, lastPoly, realLastPoly;
	std::vector<ImplicitPolygon2D *> polys;
	// TODO: remove vector
	// (for visualisation)
	std::vector<Point2D> vis;
	//
	for(int i = 0; i < nbrPts; i++)
	{
		vis.push_back(points[i]);
		ipts.push_back(i);
	}
	int id = 0, size = nbrPts, CWid = nbrPts - 1, CCWid = 0;
	bool CWpropagation = false;
	while(size >= 3)
	{		  
		if(id == -1)
			break;
		Point2D p0 = points[ipts[tmod(id, size)]],
				p1 = points[ipts[tmod(id + 1, size)]],
				p2 = points[ipts[tmod(id + 2, size)]];								
		float lto = p2.isLeftTo(p0, p1);
		if(lto == 0)
			lto = 0;
		if(lto > 0) // no degenerate here
		{
			bool noOverlap = false;
			for(int i = tmod(id + 3, size); i != id && !noOverlap; i = tmod(i + 1, size))
			{
				Point2D ipt = points[ipts[i]];
				if(ipt.exactEquals(p0)||ipt.exactEquals(p1)||ipt.exactEquals(p2))
					noOverlap = false;
				else
					noOverlap =ipt.isInCCWTriangle(p0, p1, p2);
				if(noOverlap)
					break;
			}
			if(!noOverlap)
			{	
				// build convex polygon
				int nb = 3;
				int falsenb = 3;
				lastPoly.push_back(tmod(id, size));
				lastPoly.push_back(tmod(id + 1, size));
				lastPoly.push_back(tmod(id + 2, size));
				realLastPoly.push_back(tmod(id, size));
				realLastPoly.push_back(tmod(id + 1, size));
				realLastPoly.push_back(tmod(id + 2, size));
				// CCW extantion
				Point2D lp = points[ipts[lastPoly[0]]],
						llp =  points[ipts[lastPoly[1]]],
						fp = points[ipts[lastPoly[nb - 1]]],
						sp = points[ipts[lastPoly[nb - 2]]];
				if(fp.exactEquals(sp) || sp.exactEquals(lp) || llp.exactEquals(fp))
					fp = fp;
				for(int nid = tmod(lastPoly[nb - 1] + 1, size); nid != realLastPoly[0]; nid = tmod(nid + 1, size))
				{		
					bool gob;
					Point2D np = points[ipts[nid]];
					gob = np.exactEquals(lp) || np.exactEquals(fp);
					if(np.isLeftTo(llp, lp, -1) < 0 && 
					   fp.isLeftTo(lp, np, -1) < 0	&&
					   sp.isLeftTo(np, fp, -1) < 0)
					{			  
						bool noIntercect = true;	
						
						if(!gob)
						{
						/*if(lp.exactEq
						uals(fp)||fp.exactEquals(np)||lp.exactEquals(np))
							noIntercect = true;	 // triangle dégénéré
						else
						{  */
							for(int i = tmod(nid + 1, size); i != realLastPoly[0] && noIntercect; i = tmod(i + 1, size))
							{
								Point2D ipt = points[ipts[i]];
								if(ipt.exactEquals(lp)||ipt.exactEquals(fp)||ipt.exactEquals(np))
									noIntercect = true;
								else
									noIntercect = !ipt.isInCCWTriangle(lp, fp, np);
							}
						}
						//}
						if(gob || noIntercect)
						{
							// add point
							if(!gob)
							{
								lastPoly.push_back(nid); 
								sp = fp;
								fp = np;
								falsenb++;
							}
							else
								falsenb = falsenb;
							// else gob point without adding to the polygon
							realLastPoly.push_back(nid);
							nb++;
						}
						else break;
					}
					else break;
				}														  
				// CW extantion
				fp = points[ipts[lastPoly[0]]],
				sp =  points[ipts[lastPoly[1]]],
				lp = points[ipts[lastPoly[falsenb - 1]]];
				llp = points[ipts[lastPoly[falsenb - 2]]];		  
				if(fp.exactEquals(sp) || sp.exactEquals(lp) || llp.exactEquals(fp))
					fp = fp;
				for(int nid = tmodinv(realLastPoly[0] - 1, size); nid != realLastPoly[nb - 1]; nid = tmodinv(nid - 1, size))
				{		
					bool gob;
					Point2D np = points[ipts[nid]];
					gob = np.exactEquals(lp) || np.exactEquals(fp);
					if(gob || (np.isLeftTo(llp, lp, 1) > 0 && 
					   (fp.isLeftTo(lp, np, 1) > 0 &&
					   (sp.isLeftTo(np, fp, 1) > 0))))
					{			  
						bool noIntercect = true;
						/*if(lp.exactEquals(fp)||fp.exactEquals(np)||lp.exactEquals(np))
							noIntercect = true;	 // triangle dégénéré
						else
						{  */
						if(!gob)
						{
							for(int i = tmodinv(nid - 1, size); i != realLastPoly[nb - 1] && noIntercect; i = tmodinv(i - 1, size))
							{				  
								Point2D ipt = points[ipts[i]];
								if(ipt.exactEquals(lp)||ipt.exactEquals(fp)||ipt.exactEquals(np))
									noIntercect = true;
								else
									noIntercect = !ipt.isInCWTriangle(lp, fp, np);
							}
						}
						//}
						if(gob || noIntercect)
						{
							// add point
							// add point
							if(!gob)
							{							
								lastPoly.insert(lastPoly.begin(), nid);
								sp = fp;
								fp = np;
								falsenb++;
							}
							// TODO: remove this
							else
								falsenb = falsenb;
							// else gob point without adding to the polygon		
							realLastPoly.insert(realLastPoly.begin(), nid);

							nb++;
						}
						else break;
					}
					else break;
				}
				// end of extantion
				Point2D *respts = new Point2D[falsenb];
				int realnb = falsenb;
				int offset = 0;
				for(int i = 0; i < falsenb; i ++)
				{
					Point2D pt = points[ipts[lastPoly[i]]];
					respts[i] = points[ipts[lastPoly[i]]];
				}
				for(int i = 1; i < nb - 1; i ++)
				{
					int pivot = realLastPoly[i];
					ipts.erase(ipts.begin() + pivot);
					for(int j = i + 1; j < nb - 1; j ++)
					{
						if(realLastPoly[j] > pivot)
							realLastPoly[j]--;
					}
				}
				size -= nb - 2;
				polys.push_back(new ImplicitPolygon2D(respts, realnb, this, (int)polys.size() + 1));
				lastPoly.clear();
				realLastPoly.clear();
				id = 0;
				CWid = size - 1;
				CCWid = 0;
				CWpropagation = false;
			}
			else
			{				  
				CWpropagation = !CWpropagation;
				if(CWpropagation)
				{
					id = CWid;
					CWid = tmodinv(CWid - 1, size);	  // le décrémant vient après				
				}
				else
				{
					CCWid = tmod(CCWid + 1, size);;  // l'incrément vient avant
					id = CCWid;
				}
			}
		}
		else
		{
			CWpropagation = !CWpropagation;
			if(CWpropagation)
			{
				id = CWid;
				CWid--;	  // le décrémant vient après				
			}
			else
			{
				CCWid++;  // l'incrément vient avant
				id = CCWid;
			}
		}
	}
	ipts.clear();
	lastPoly.clear();
	realLastPoly.clear();
	if(nbrSubShapes > 0)
	{	  
		for(int i = 0; i < nbrSubShapes; i++)
			delete subShapes[i];
		delete[] subShapes;
	}
	nbrSubShapes = (int)polys.size();			 
	subShapes = new ImplicitPolygon2D*[nbrSubShapes];
	for(int i = 0; i < nbrSubShapes; i++)
		subShapes[i] = polys[i];
	polys.clear();
		
}

void Polygon2D::buildOBBtree()
{	
	otree = new OBBtree(0, 0, chull->getOBB());
	if(nbrSubShapes > 1)
	{
		// divide space		  
		std::vector<ImplicitPolygon2D *> rightset;
		std::vector<ImplicitPolygon2D *> leftset;
		Point2D ao;
		Point2D as;
		otree->o->getMedialAxis(&ao, &as);
		for(int i = 0; i < nbrSubShapes; i++)
		{
			Vector2D vp = subShapes[i]->getCenter();
			Point2D p(vp.getX(), vp.getY());
			if(p.isLeftTo(ao, as) < 0)
				leftset.push_back(subShapes[i]);
			else
				rightset.push_back(subShapes[i]);
		}
		if(rightset.size()==0)
		{
			int ls = ((int)leftset.size()) / 2;
			while(leftset.size() != ls)
			{
				rightset.push_back(leftset[leftset.size()-1]);
				leftset.pop_back();
			}
		}
		else if(leftset.size() == 0)
		{	 
			int ls = ((int)rightset.size()) / 2;
			while((int)rightset.size() != ls)
			{
				leftset.push_back(rightset[(int)rightset.size()-1]);
				rightset.pop_back();
			}
		}
		// rego	
		int id = 0;
		if((int)rightset.size()!=0)
			buildOBBtree(&(otree->r), rightset, id);
		else
			otree->r = 0;
		if((int)leftset.size()!=0)
			buildOBBtree(&(otree->l), leftset, id);
		else
			otree->l = 0;
	}
}

void Polygon2D::buildOBBtree(OBBtree **o, std::vector<ImplicitPolygon2D*> &polyset, int &id)
{		
	id++;
	int s = (int)polyset.size();
	if(s == 1)
	{
		*o = new OBBtree(0,0, polyset[0]->getOBB());
		polyset.clear();
		return;
	}
	Vector2D ya(0,-1);
	std::vector<Point2D> chpts;
	// calc subshapes' convex hull
	Point2D miny, studiedpt;
	polyset[0]->getSupportPoint(ya, &miny);
	miny = polyset[0]->toLocal(miny);
	for(int i = 1; i < s; i++)
	{
		Point2D tmp;	
		polyset[i]->getSupportPoint(ya, &tmp);	   
		tmp = polyset[i]->toLocal(tmp);
		if(tmp.getY() < miny.getY())
			miny = tmp;
	}				
	studiedpt = miny;
	do
	{
		chpts.push_back(studiedpt);
		Point2D rp = polyset[0]->rightTgtPt(studiedpt);
		for(int i = 1; i < s; i++)
		{
			Point2D tmp = polyset[i]->rightTgtPt(studiedpt); 
			if(tmp.isLeftTo(studiedpt, rp) < 0)
				rp = tmp;			
		}	
		studiedpt = rp;
	} while(studiedpt.getX() != miny.getX() || studiedpt.getY() != miny.getY());
	int chn = (int)chpts.size();
	Point2D *ch = new Point2D[chn];
	for(int i = 0; i < chn; i++)
		ch[i] = chpts[i];
	chpts.clear();
	// build obb	
	*o = new OBBtree(0, 0, ImplicitPolygon2D::buildOBB(ch, chn, chull, -1));
	OBBtree *oo = *o;

	// divide space		  
	std::vector<ImplicitPolygon2D *> rightset;
	std::vector<ImplicitPolygon2D *> leftset;
	Point2D ao;
	Point2D as;
	oo->o->getMedialAxis(&ao, &as);
	for(int i = 0; i < s; i++)
	{
		Vector2D vp = polyset[i]->getCenter();
		Point2D p(vp.getX(), vp.getY());
		if(p.isLeftTo(ao, as) < 0)
			leftset.push_back(polyset[i]);
		else
			rightset.push_back(polyset[i]);
	}
	if(rightset.size()==0)
	{
		int ls = ((int)leftset.size()) / 2;
		while(leftset.size() != ls)
		{
			rightset.push_back(leftset[leftset.size()-1]);
			leftset.pop_back();
		}
	}
	else if(leftset.size() == 0)
	{	 
		int ls = ((int)rightset.size()) / 2;
		while(rightset.size() != ls)
		{
			leftset.push_back(rightset[rightset.size()-1]);
			rightset.pop_back();
		}
	}
	// rego	
	polyset.clear();
	if(rightset.size()!=0)
		buildOBBtree(&(oo->r), rightset, id);
	if(leftset.size()!=0)
		buildOBBtree(&(oo->l), leftset, id);
}


int Polygon2D::simplify(Point2D *in, int n, Point2D **out, int tolerence)
{
	Point2D *pt = new Point2D[n];
	int s = 0;
	int p=n-2, np=n-1, nnp=0;
	while(nnp<n)
	{
		float xa =in[p].getX(),
		      xc = in[np].getX(),
		      xb = in[nnp].getX(),
		      ya = in[p].getY(),
		      yc = in[np].getY(),
		      yb = in[nnp].getY();
		if(abs(xa*yc-xa*yb+xb*ya-xb*yc+xc*yb-xc*ya) >= 2 * tolerence)
		{
			pt[s] = in[np];
			s++;
			p = np;
			np = nnp;
		}
		else
			np = nnp;	 
		nnp++;
	}
	*out = new Point2D[s];
	for(int i=0;i<s;i++)
		(*out)[i] = pt[i];
	delete[] pt;
    return s;
}

float Polygon2D::getInertiaMomentum(float m)
{
	float num = 0, denum = 0;
	int l = nbrPts, j = l - 1;
	for (int i = 0; i < l; i++)
	{
		Vector2D pn = Vector2D(points[j]);
		Vector2D pn1 = Vector2D(points[i]);
		float f = pn1.cross(pn).magnitude();
		denum += f;
		num += f * (pn1.dot(pn1) + pn1.dot(pn) + pn.dot(pn));
		j = i;
	}
	return (denum == 0) ? 0 : abs(m * num / (6 * denum));
}
float Polygon2D::getSurface()
{ return Polygon2D::getSurface(points, nbrPts); }

#pragma region Static methods

	struct PointWithNext
	{
		Point2D pt;
		struct PointWithNext *next, *homolog;
		float bparam;
		inline PointWithNext(Point2D pt)
		{ this->pt = pt; next = this; homolog = 0; bparam = -1; }
	};
int Polygon2D::getUncrossedPolygon(Point2D *pts, int nb, Point2D **res)
{
	// create initial chain
		// find an external point
	int mostext = 0;
	for(int i = 1; i < nb; i++)
	{
		if(pts[i].getY() < pts[mostext].getY())
			mostext = i;
		i++;
	}
		// create chain with mostext at initial point
	struct PointWithNext *head = new struct PointWithNext(pts[mostext]);
	struct PointWithNext *prev = head;
	int j = (mostext == nb - 1) ? 0	: mostext + 1;
	while(j != mostext)
	{
		prev->next = new struct PointWithNext(pts[j]);
		prev = prev->next;
		if(j == nb - 1)
			j = 0;														
		else
			j++;
	}
	prev->next = head; // make it circular
	// find all intersection and insert them in the chain
	struct PointWithNext *currref   = head, *ncurrref;
	struct PointWithNext *currscan, *ncurrscan;	
	int nbinter = 0;
	do
	{
		// find next currref  
		ncurrref = currref->next;
		while(ncurrref->homolog != 0)
			ncurrref = ncurrref->next;
		if(ncurrref == head)
			break; // don't test last edge as base
		// find currscan	  
		currscan = ncurrref->next;	 // skip next epge
		while(currscan->homolog != 0)
			currscan = currscan->next;
		// find intersections
		while(currscan != head)
		{
			// find next currscan
			ncurrscan = currscan->next;
			while(ncurrscan->homolog != 0)
				ncurrscan = ncurrscan->next;
			if(ncurrscan == currref)
				break;
			// find intersection
			Point2D inter;
			float bparamscan;
			float bparamref = Point2D::intersectSegments(currref->pt,ncurrref->pt,currscan->pt, ncurrscan->pt,&inter,&bparamscan);
			if(bparamref != -1)
			{			
				nbinter += 2;
				// if intersects, insert in chain sorting by the barycentric coord
				struct PointWithNext *inter1 = new struct PointWithNext(inter);	  
				struct PointWithNext *inter2 = new struct PointWithNext(inter); 
				inter1->bparam = bparamref;
				inter2->bparam = bparamscan;
				inter1->homolog = inter2;
				inter2->homolog = inter1;
				// insert 1 in chain
				struct PointWithNext *previnsert = currref;
				while(previnsert->next->homolog != 0 && previnsert->next->bparam < bparamref)
					previnsert = previnsert->next;
				inter1->next = previnsert->next;
				previnsert->next = inter1;
				// insert 2 in chain
				previnsert = currscan;
				while(previnsert->next->homolog != 0 && previnsert->next->bparam < bparamscan)
					previnsert = previnsert->next;
				inter2->next = previnsert->next;
				previnsert->next = inter2;
				// end of insersion

			}
			// else no intersection
			currscan = ncurrscan;
		}
		currref = ncurrref;
	}
	while(currref != head);
	// now parc links and revert links
	currref = head->next; // skip head: cannot be an intersection point
	while(currref != head)
	{
		if(currref->homolog != 0)
		{   
			struct PointWithNext *prevparc = currref->next;
			struct PointWithNext *parc = prevparc->next;
			// invert links to homolog	
			while(parc != currref->homolog)
			{					
				struct PointWithNext *n = parc->next;
				parc->next = prevparc;
				prevparc = parc;
				parc = n;
			}
			currref->next->next = currref->homolog;
			currref->next = prevparc;
			currref->homolog->homolog = 0; // never reinterpret it again
			currref->homolog = 0; // never reinterpret it again
		}
		currref = currref->next;
	}

	// the polygon is now uncrossed, copy the result  
	// and free memory
	currref = head;
	int l = nbinter + nb; 
    *res = new Point2D[l];
	int i = 0;
	while(i<l)
	{
		struct PointWithNext *toDestroy;
		toDestroy = currref;
		(*res)[i] = currref->pt;
		currref = currref->next;
		// free
		delete toDestroy;
		i++;
	}
	return l;
}
/////////////////////////////////////////////////
//				MELKMAN ALGORITHM			   //
// Builds convex hull of current polygon's	   //
// boundary's polyline representation.		   //
// Only handles simple polygon. (i.e. w/o	   //
// self intersection.)						   //
// Outputs an indexes-array instead of		   //
// Point2D array.							   //
// Returns the number of indexes in the array. //
/////////////////////////////////////////////////
int Polygon2D::buildConvexHull(Point2D *pts, int nbPts,Point2D ** outHull)
{
	int res;
	int *D=new int[2*nbPts+1];
	int bot = nbPts - 2, top = bot + 3;
	D[bot] = D[top] = 2;
	if(pts[2].isLeftTo(pts[0],pts[1]) > 0)
	{
		D[bot+1] = 0;
		D[bot+2] = 1;
	}
	else
	{
		D[bot+2] = 0;
		D[bot+1] = 1;
	}
	int i=3;
	while(i < nbPts)
	{
		Point2D p = pts[i];
		if(p.isLeftTo(pts[D[bot]],pts[D[bot + 1]]) <= 0 
		   || p.isLeftTo(pts[D[top - 1]],pts[D[top]]) <= 0)
		{
			while(p.isLeftTo(pts[D[bot]],pts[D[bot + 1]]) <= 0)
				bot++;
			bot--;
			D[bot] = i;

			while(p.isLeftTo(pts[D[top - 1]],pts[D[top]]) <= 0)
				top--;
			top++;
			D[top] = i;
		}
		i++;
	}
	res = top - bot;
	*outHull = new Point2D[res];
	for(i=bot;i<=top;i++)
		(*outHull)[i - bot] =  pts[D[i]]; 
	delete[] D;
	return res;
}
float Polygon2D::getSurface(Point2D *in, int n)
{	  
	int l = n;
	float a = 0;
	int j = l - 1;
	for (int i = 0; i < l; i++)
	{
		a += in[j].getX() * in[i].getY() - in[j].getY() * in[i].getX();
		j = i;
	}
	return abs(a / 2);
}

Point2D Polygon2D::getCentroid(Point2D *in, int n)
{  return Polygon2D::getCentroid(in, n, Polygon2D::getSurface(in, n)); }

Point2D Polygon2D::getCentroid(Point2D *in, int n,float aire)
{
	int l = n;
	float ax = 0, ay = 0;
	int j = l - 1;
	for (int i = 0; i < l; i++)
	{
		float ix = in[i].getX(), iy = in[i].getY(), jx = in[j].getX(), jy = in[j].getY();
		float det = (jx * iy - jy * ix);
		ax += (jx + ix) * det;
		ay += (jy + iy) * det;
		j = i;
	}
	return Point2D(ax / (6 * aire), ay / (6 * aire));
}
#pragma endregion
#pragma endregion

#pragma region ImplicitPolygon2D
ImplicitPolygon2D::ImplicitPolygon2D(Point2D *globalPts, int n, Polygon2D *p, int id) 
	: nbrPts(n) 
{
	parent = p; 
	margin = PROXIMITY_AWARENESS + 0.5f;
	pts = globalPts;		
	center = Polygon2D::getCentroid(pts, n);
	obb = ImplicitPolygon2D::buildOBB(pts, n, this, id);
	radius = sqrt(_getBoundingSphereSqRadius());
}

float ImplicitPolygon2D::_getBoundingSphereSqRadius()
{
	float res = 0;
	for(int i = 0; i<nbrPts; i++)
	{
		Vector2D v(center, pts[i]);
		float vv = v * v;
		if(vv > res)
			res = vv;
	}
	return res;
}

float ImplicitPolygon2D::getBoundingSphereRadius()
{ return radius; }

Point2D ImplicitPolygon2D::rightTgtPt(Point2D &ref)
{	   	
	int a = 0, b = nbrPts - 1, ires;
    if(pts[a].equals(ref))
		ires = 1;		
    else if(pts[b].equals(ref))
		ires = a;
	else if(pts[a].isLeftTo(ref, pts[a+1]) < 0	&& pts[a].isLeftTo(ref, pts[b]) <= 0)
		ires = a;  	
	else if(pts[b].isLeftTo(ref, pts[a]) < 0	&& pts[b].isLeftTo(ref, pts[b - 1]) <= 0)
		ires = b;
	else
	{
		while(true)
		{
			int c = (a + b)/2;
						
			bool dnc = pts[c].isLeftTo(ref, pts[c+1]) < 0;   
			if(pts[c].equals(ref))
			{
				ires = c + 1;	
				break;
			}
			if(dnc	&& pts[c].isLeftTo(ref, pts[c-1]) <= 0)
			{
				ires = c;
				break;
			}
			float upa = pts[a].isLeftTo(ref, pts[a+1])  > 0;
			if(upa)
			{
				if(dnc)
					b = c;
				else
				{
					if(pts[c].isLeftTo(ref, pts[a])  > 0)
						b = c;
					else
						a = c;
				}
			}
			else
			{
				if(!dnc)
					a = c;
				else
				{   
					if(pts[c].isLeftTo(ref, pts[a]) < 0)
						b = c;
					else
						a = c;
				}
			}
		}
	}
	return pts[ires];
} 

int ImplicitPolygon2D::naiveClimb(int ibase, int imax, Vector2D &v)
{
	float lastDot = -FLT_MAX, dx = v.getX(), dy = v.getY();
	int i, ires = ibase;
	for(i = ibase; i <= imax; i++)
	{
		float dot = pts[i].getX() * dx + pts[i].getY() * dy;
		//if(dot < lastDot)
		//	break;
		if(dot > lastDot)
		{
			ires = i;
			lastDot = dot;
		}
	}
	return ires;
}

////////////////////////////////////////////
//			 BINARY SEARCH                //
//									      //
// Retrieves s(d) suporting point of      //
// current convex polygon using binary    //
// search. Algorithm is O(log(n))         //
////////////////////////////////////////////
int ImplicitPolygon2D::getSupportPoint(Vector2D &od, Point2D *res)
{		   
	Vector2D d = toRotatedInv(od);
	int ires;
	if(nbrPts < 8) // n - 1 < 3 * log(n) pour n < 8
		ires = naiveClimb(0, nbrPts - 1, d);
	else
	{
		int a = 0, b = nbrPts - 1;
		float dx = d.getX(), dy = d.getY();
		Vector2D va = Vector2D(pts[0], pts[1]),
				 vb = Vector2D(pts[b], pts[0]),
				 vb_1 = Vector2D(pts[b-1],pts[b]); 
		bool upa = va.dot(d) > 0;
		bool upb = vb.dot(d) > 0;
		if(upb)
		{
			if(!upa)
			{
				*res = toGlobal(pts[0]);
				return 0;
			}
		}
		else if(vb_1.dot(d) > 0)
		{
			*res = toGlobal(pts[b]);
			return b;
		}
		while(true)
		{
			int c = (a + b)/2;
			Vector2D vc(pts[c], pts[c+1]),
					 vc_1(pts[c-1],pts[c]);
						
			bool upc = vc.dot(d) > 0;
			if(!upc	&& vc_1.dot(d) > 0)
			{
				ires = c;
				break;
			}

			if(!upa)
			{
				if(upc)
				{
					a = c;
					upa = upc;
				}
				else
				{
					Vector2D vac(pts[a],pts[c]);
					if(vac.dot(d) < 0)
					{
						a = c;
						upa = upc;
					}
					else
						b = c;
				}
			}
			else
			{
				if(!upc)
					b = c;
				else
				{   
					Vector2D vac(pts[a],pts[c]);
					if(vac.dot(d) < 0)
						b = c;
					else
					{
						a = c;
						upa = upc;
					}
				}
			}
		}
	}	   
	*res = toGlobal(pts[ires]);
	return ires;
}

////////////////////////////////////////////
//			 HILL CLIMBING                //
//									      //
// Retrieves s(d) suporting point of      //
// current convex polygon using Hill      //
// Climbing. Algorithm is O(n).           //
// Setting o with last int returned by	  //
// any getSupportPoint overload may allow //
// this algorithm to run in near-constant //
// time (geometric coherence).            //
////////////////////////////////////////////
int ImplicitPolygon2D::getSupportPoint(Vector2D &od, Point2D *res, int o)
{
	Vector2D d = toRotatedInv(od);
	int n = tmod(o + 1, nbrPts);
	int p = tmodinv(o - 1, nbrPts);
	float dx = d.getX(),
		  dy = d.getY();									   
	float doto = pts[o].getX() * dx + pts[o].getY() * dy;
	float dotp = pts[p].getX() * dx + pts[p].getY() * dy;
	float dotn = pts[n].getX() * dx + pts[n].getY() * dy;
	if(dotn > doto  || dotp > doto)
	{
		if(dotn < dotp)
		{
			do
			{   
				dotn = dotp;
				o = p;
				ilow(p);
				dotp = pts[p].getX() * dx + pts[p].getY() * dy;
			} while(dotp >= dotn); 
		}
		else
		{
			do
			{   
				dotp = dotn;
				o = n;
				ihig(n);
				dotn = pts[n].getX() * dx + pts[n].getY() * dy;
			} while(dotn >= dotp);
		}
	}
	*res = toGlobal(pts[o]);
	return o;
}

//////////////////////////////////////////
//			ROTATIVE CALIPERS		    //
//								        //
// Calculate current convex polygon's   //
// OBB using O(n) rotative calipers     //
// algorithm.                           //
//////////////////////////////////////////
OBB *ImplicitPolygon2D::buildOBB(Point2D *pts, int nbrPts, ImplicitPolygon2D *parent, int id)
{		
	int t_p[4];   
	float t_teta[4];
	int obbl, obbd;
	float obblon, obblar;
	float obbTeta = 0;
	float teta = 0, minAire = FLT_MAX;
	float siteta = 0, coteta = 1;
	Vector2D calipers[4];
	calipers[0] = Vector2D(-1,0);
	calipers[1] = Vector2D(1,0);
	calipers[2] = Vector2D(0,1);
	calipers[3] = Vector2D(0,-1);
	for(int i = 0;i < 4;i++)
		t_p[i] = 0;
	// calcul de l'AABB
	for(int i = 1;i<nbrPts;i++)
	{
		Point2D p = pts[i];
		if(p.getX() > pts[t_p[2]].getX())
			t_p[2] = i;
		else if (p.getX() < pts[t_p[3]].getX())
			t_p[3] = i;
		
		if(p.getY() > pts[t_p[0]].getY())
			t_p[0] = i;
		else if (p.getY() < pts[t_p[1]].getY())
			t_p[1] = i;
	}
	for(int i=0; i<4; i++)
	{
		Vector2D vc = Vector2D(pts[t_p[i]],pts[tmod(t_p[i]+1,nbrPts)]);
		t_teta[i] = acos(vc.dot(calipers[i])/vc.magnitude());
	}
	do
	{
		Vector2D ud = Vector2D(pts[t_p[0]],pts[t_p[1]]);
		Vector2D rl = Vector2D(pts[t_p[2]],pts[t_p[3]]);
		float lon = abs(ud.dot(calipers[3]));
		float lar = abs(rl.dot(calipers[0]));
		float naire = lon * lar;
		if(naire < minAire)
		{
			minAire = naire;	  
			obbl = t_p[3];	  
			obbd = t_p[1];
			obblon = lon;
			obblar = lar;
			obbTeta = teta;
		}  

		float minTeta = 90;
		for(int i=0; i<4; i++)
		{
			while (t_teta[i] < 0.0000000001f)
			{
				t_p[i] = tmod(t_p[i] + 1,nbrPts);
				int id = t_p[i];
				Vector2D vc = Vector2D(pts[id],pts[tmod(id+1,nbrPts)]);
				t_teta[i] = acos(vc.dot(calipers[i])/vc.magnitude());
			}
			if(t_teta[i] < minTeta)
				minTeta = t_teta[i];							
		}
		teta += minTeta;		 
		coteta = cos(teta);
		siteta = -sin(teta); 
		calipers[0] = Vector2D(-coteta, siteta);
		calipers[1] = Vector2D(coteta, -siteta);
		calipers[2] = Vector2D(siteta, coteta);
		calipers[3] = Vector2D(-siteta, -coteta);
		for(int i = 0; i < 4; i++)
			t_teta[i]-= minTeta;
	} while(teta < M_PI / 2);
	// build OBB	   
	// on se base sur D et L
	Point2D origin;	   
	Vector2D repY = Vector2D(-sin(-obbTeta),-cos(-obbTeta));
	Vector2D repX = Vector2D(-repY.getY(),repY.getX());
	if(obbl == obbd)
		origin = pts[obbd];
	else
	{
		Vector2D dl = Vector2D(pts[obbl],pts[obbd]);
		float dotp	= abs(repY * dl);
 		origin = pts[obbl] + (repY * dotp);
	}
	float m = parent->getMargin();
	obblon +=  m;
	obblar += m;
	Point2D opp = origin - repY * obblon + repX * obblar;
	obblon +=  m;
	obblar += m;
	origin =  opp + repY * obblon - repX * obblar;
	return new OBB(origin, origin - repY * obblon,opp , origin + repX * obblar, parent, minAire, id);
}	
#pragma endregion