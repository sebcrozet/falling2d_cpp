#include "stdafx.h"
#include "DistanceSolver.h"
#include <stdlib.h>

#pragma region GJKsolver
GJKsolver::GJKsolver(ImplicitShape &a, ImplicitShape &b) : A(a), B(b), simplexSize(0)
{
	satlastdir = b.getCenter() - a.getCenter();
	simplexSize = 1;
	dirs[0] = satlastdir;
	msum = a.getMargin() + b.getMargin();
	sqmsum = msum * msum;
}

bool GJKsolver::canDestroy()
{
	Vector2D v = A.getCenter() - B.getCenter();
	float rr = A.getBoundingSphereRadius() + B.getBoundingSphereRadius() + msum + (float)GJK_DESTROYLIMIT;
	rr = rr*rr;
	float vv = v*v;
	return (v*v > rr);
}

bool GJKsolver::_solve(std::vector<SubCollision> &res)
{
	Point2D pa, pb;
	if(getPenDepth(&pa, &pb) != 0)
	{
		res.push_back(SubCollision(pa, pb));
		return false;
	}
	return canDestroy();
}

float GJKsolver::getPenDepth(Point2D *pA, Point2D *pB)
{
	float bparam;
	float dist = solveDist(&bparam);
	if(dist != -1)
	{
		if(dist != 0)
		{
			if(bparam == 0)
			{
				*pA = ptsA[0];
				*pB = ptsB[0];
			}
			else
			{
				*pA = ptsA[0] + (Vector2D(ptsA[0], ptsA[1]) * (bparam));
				*pB = ptsB[0] + (Vector2D(ptsB[0], ptsB[1]) * (bparam));	
			}
			Vector2D ab = Vector2D(*pA, *pB);
			ab.normalise();
			*pA += ab * A.getMargin();
			*pB += ab * (-B.getMargin());	
			ab = Vector2D(*pA, *pB);
			float nnn = ab.magnitude();
			nnn++;
		   //printf("%f\n",bparam);
		}
	}
	else
	{
		if(!gjk_buildMarginedSimplexWithOrigin()) 
			return 0; // error in GJK precision (origin not in the cso)
		EPAsolver es(*this);
		dist = (es.getPenetrationDepth(pA, pB).magnitude());
	}
	return dist;
}


void  GJKsolver::updateClosestFeatureSimplexDatas(Vector2D &p, float * barycentricParam)
{
	if(simplexSize == 1)
		{
			p = ptsC[0];
			*barycentricParam = 0;
			simplexSize = 2;
		}
		else if(simplexSize == 2) 
		{
#pragma region Closest point to a line segment
			Vector2D ab = ptsC[1] - ptsC[0];
			Vector2D ao = ptsC[0].reflexion();
			*barycentricParam = ab * ao;
			if(*barycentricParam <= 0)
			{
				*barycentricParam = 0;
				p = ptsC[0];
			}
			else
			{
				float sq = ab * ab;
				if(*barycentricParam >= sq)
				{
					*barycentricParam = 0;
					p = ptsC[1];
					swapPts(ptsC[0], ptsC[1]);	
					swapPts(dirs[0], dirs[1]);	
					ptsA[0] = ptsA[1];	
					ptsB[0] = ptsB[1];						
				}
				else
				{
					*barycentricParam /= sq;
					p = ptsC[0] + (ab * (*barycentricParam));
					simplexSize = 3;
				}
			}
#pragma endregion
		}
		else 
		{
#pragma region Closest point to triangle
			// ptsC[0] = A , 1 = B, 2 = C
			Vector2D ab = ptsC[1] - ptsC[0];
			Vector2D ac = ptsC[2] - ptsC[0];
			Vector2D ap = ptsC[0].reflexion();
			float d1 = ab * ap;
			float d2 = ac * ap;
			if(d1 <= 0 && d2 <= 0)
			{
				*barycentricParam = 0;
				p = ptsC[0];
				simplexSize = 2;
			}
			else
			{
				Vector2D bp = ptsC[1].reflexion();
				float d3 = ab * bp;
				float d4 = ac * bp;
				if(d3 >= 0 && d4 <= d3)
				{   
					*barycentricParam = 0;
					p = ptsC[1];
					swapPts(ptsC[0], ptsC[1]);  
					swapPts(dirs[0], dirs[1]);		
					ptsA[0] = ptsA[1];	
					ptsB[0] = ptsB[1];
					simplexSize = 2;	
				}
				else
				{
					float vc = d1*d4 - d3*d2;
					if(vc <= 0 && d1 >= 0 && d3 <= 0)
					{
						*barycentricParam = d1 / (d1 - d3);
						p = ptsC[0] + ab * (*barycentricParam);
					}
					else
					{
						Vector2D cp = ptsC[2].reflexion();
						float d5 = ab * cp;
						float d6 = ac * cp;
						if(d6 >= 0 && d5 <= d6)
						{				  
							*barycentricParam = 0;
							p = ptsC[2];
							swapPts(ptsC[0], ptsC[2]);	  
							swapPts(dirs[0], dirs[2]);	
							ptsA[0] = ptsA[2];	
							ptsB[0] = ptsB[2];
							simplexSize = 2;	
						}
						else
						{	// ac
							float vb = d5*d2 - d1*d6;
							if(vb <= 0 && d2 >= 0 && d6 <= 0)
							{	
								*barycentricParam = d2 / (d2 - d6);
								p = ptsC[0] + ac * (*barycentricParam);	  
								swapPts(ptsC[1], ptsC[2]);	
								swapPts(dirs[1], dirs[2]);
								ptsA[1] = ptsA[2];	
								ptsB[1] = ptsB[2];
							}
							else
							{
								// bc
								float va = d3 * d6 - d5 * d4;
								if(va<=0 && (d4-d3)>=0 && (d5-d6)>=0)
								{	   
									//*barycentricParam = d2 / (d2 - d6);
									*barycentricParam = (d4-d3) / ((d4-d3) + (d5 - d6));
									p = ptsC[1] + (ptsC[2] - ptsC[1]) * (*barycentricParam);	  
									swapPts(ptsC[0], ptsC[2]);	  
									swapPts(dirs[0], dirs[2]);	
									ptsA[0] = ptsA[2];	
									ptsB[0] = ptsB[2];
									*barycentricParam = 1.f- *barycentricParam;

								}
								else
									simplexSize = 4;
							}
						}
					}
				}
			}
#pragma endregion
		}
}
	
bool GJKsolver::gjk_buildMarginedSimplexWithOrigin()
{	
	// see if slight modifications on current simplex works
	if(simplexSize == 3)
	{
		if(!Point2D().isInUnorientedTriangle(Point2D(ptsC[0].getX(),ptsC[0].getY()), Point2D(ptsC[1].getX(),ptsC[1].getY()), Point2D(ptsC[2].getX(),ptsC[2].getY())))
			return false;
		for(int i = 0; i < 3; i++)
		{
			Vector2D ddir = dirs[i].direction();
			A.addMargin(ddir.reflexion(), &ptsA[i]);
			B.addMargin(ddir, &ptsB[i]);
			ptsC[i] = Vector2D(ptsA[i] - ptsB[i]);
		}
		if(Point2D().isInUnorientedTriangle(Point2D(ptsC[0].getX(),ptsC[0].getY()), Point2D(ptsC[1].getX(),ptsC[1].getY()), Point2D(ptsC[2].getX(),ptsC[2].getY())))
			return true;
		else
			simplexSize = 1;

	}
	// rerun gjk with margin
	bool notIn = true;
	Vector2D p(0, 1);
	simplexSize = 1;
	int fullSimplexSize = simplexSize;
	float barycentricParam;
	do
	{
		Vector2D cPoint;
		Point2D cp1, cp2;
		int optid1, optid2;
		Vector2D np = p.direction();
		optid1 = A.getMarginedSupportPoint(np.reflexion(), &cp1);
		optid2 = B.getMarginedSupportPoint(np, &cp2);
		cPoint = Vector2D(cp1 - cp2);
		bool stop = false;
		for(int i = simplexSize-1; i>=0;i--)
		{
			if(ptsC[i].equals(cPoint))
			{
				stop = true; // wont converge any more
				break;
			}
		}
		if(stop)
		{
			notIn = false;
			simplexSize = fullSimplexSize;
			return true;
		}	
		fullSimplexSize = simplexSize;
		int id = simplexSize-1;
		ptsA[id] = cp1;
		ptsB[id] = cp2;
		ptsC[id] = cPoint;
		optidsA[id] = optid1;
		optidsB[id] = optid2;
		p.setX(0); p.setY(0);					  
		updateClosestFeatureSimplexDatas(p, &barycentricParam);
		float vv = p * p;
		float dotp = p*cPoint;
		/*if((dotp > 0 && dotp * dotp > sqmsum * vv))	// no itersection
		{
			// no intersection
			simplexSize--;// = fullSimplexSize;
			return;
		}*/
		if(vv < FLT_EPSILON)	// origin included in the simplex
		{
			notIn = false;
			simplexSize--;// = fullSimplexSize;
			if(!Point2D().isInUnorientedTriangle(Point2D(ptsC[0].getX(),ptsC[0].getY()), Point2D(ptsC[1].getX(),ptsC[1].getY()), Point2D(ptsC[2].getX(),ptsC[2].getY())))
				return false;
			return true;
		}	
	} while(notIn);

	if(!Point2D().isInUnorientedTriangle(Point2D(ptsC[0].getX(),ptsC[0].getY()), Point2D(ptsC[1].getX(),ptsC[1].getY()), Point2D(ptsC[2].getX(),ptsC[2].getY())))
		return false;
	return true;
}

void GJKsolver::recomputeSimplex()
{
	//recompute last simplex datas
	if(simplexSize == 4)
		simplexSize = 3;
	for(int i = 0; i < simplexSize;i++)
	{
		Point2D cp1, cp2;
		optidsA[i] = A.getSupportPoint(dirs[i].reflexion(), &cp1);
		optidsB[i] = B.getSupportPoint(dirs[i], &cp2);
		ptsC[i] = Vector2D(cp1 - cp2);
		ptsA[i] = cp1;
		ptsB[i] = cp2;
	}
	//adjust datas
	if(simplexSize == 2)
	{
		if(optidsA[0] == optidsA[1] && optidsB[0] == optidsB[1])
			simplexSize = 1; 
	}
	else if(simplexSize == 3)
	{
		if(optidsA[0] == optidsA[1] && optidsB[0] == optidsB[1])
		{
			if(optidsA[0] == optidsA[2] && optidsB[0] == optidsB[2])
				simplexSize = 1; 
			else
			{
				optidsA[1] = optidsA[2];
				optidsB[1] = optidsB[2];
				ptsC[1] = ptsC[2];
				ptsA[1] = ptsA[2];
				ptsB[1] = ptsB[2];
				simplexSize = 2; 
			}
		}
		else if(optidsA[0] == optidsA[2] && optidsB[0] == optidsB[2])
			simplexSize = 2; 
		else if(optidsA[1] == optidsA[2] && optidsB[1] == optidsB[2])
			simplexSize = 2; 
		//else simplex valid 
	}
}

float GJKsolver::solveDist(float* bparam)
{
	bool notMax = true;		
	float barycentricParam = 0;
	Vector2D p = satlastdir;
	int iter = 0;
	float vv = FLT_MAX;
	float res = 0;
	/*
	dirs[0] = Vector2D(-1);
	simplexSize = 1;
	*/
	int fullSimplexSize = simplexSize; // will often be = simplexSize. Will be != at the beginning (here) or when simplex reduction is 3 -> 1 (when origin is in a triangle's point voronoy region) 
	
	recomputeSimplex();
	do
	{
		iter++;
		p.setX(0); p.setY(0);				  
		updateClosestFeatureSimplexDatas(p, &barycentricParam);
		vv = p * p;
		if(vv < FLT_EPSILON)	// origin included in the simplex
		{
			notMax = false;
			simplexSize--;// = fullSimplexSize;
			res = -1;
			*bparam = barycentricParam;
			return -1;
		}	
		Vector2D cPoint;
		Point2D cp1, cp2;
		int optid1, optid2;
		optid1 = A.getSupportPoint(p.reflexion(), &cp1);
		optid2 = B.getSupportPoint(p, &cp2);
		cPoint = Vector2D(cp1 - cp2);
		satlastdir = p;
		bool stop = false;
		for(int i = fullSimplexSize-1; i>=0;i--)
		{
			if(ptsC[i].equals(cPoint))
			{
				stop = true;
				break;
			}
		}
		float dotp = p*cPoint;
		if((dotp > 0 && dotp * dotp > sqmsum * vv))	// no itersection
		{
			// no intersection
			simplexSize = fullSimplexSize;
			res = 0;
			*bparam = barycentricParam;
			return 0;
		}
		if(stop || vv - dotp < EPSILON_ * EPSILON_ * vv)
		{
			// intersects in margin
			notMax = false;
			res = msum - sqrt(vv);
			simplexSize = fullSimplexSize;
			if(res<0)
				return 0;
			break;
		}
		fullSimplexSize = simplexSize;
		int id = simplexSize-1;
		ptsA[id] = cp1;
		ptsB[id] = cp2;
		ptsC[id] = cPoint;
		optidsA[id] = optid1;
		optidsB[id] = optid2;
		dirs[id] = p;
	} while(notMax);
	//printf("%i",iter);
	//printf("\n");
	*bparam = barycentricParam;
	return res;
}

#pragma endregion  // GJKsolver
		   				
#pragma region SimplexSeg
bool simplexSeg_less::operator ()(SimplexSeg *a, SimplexSeg *b) const
{
	return *a < *b;
}

SimplexSeg::SimplexSeg(Point2D &pA1, Point2D &pB1, Point2D &pC1, Point2D &pA2, Point2D &pB2, Point2D &pC2)
{
	Vector2D ab(pC1, pC2), ao(-pC1.getX(),-pC1.getY());
	dist = ab * ao;
	if(dist >= 0)
	{
		float p = ab * ab;
		if(dist > p)
			dist = -1;
		else
		{
			ptA1 = pA1;
			ptB1 = pB1;
			ptC1 = pC1;
			ptA2 = pA2;
			ptB2 = pB2;
			ptC2 = pC2;						
			bCoord = dist / p;
			//ao * ao - dist * bCoord;
			v = (ab * bCoord) - ao;
			dist = v * v;
		}
	}
}

bool SimplexSeg::isValid()
{ return dist >= 0 ;}

bool SimplexSeg::operator <(SimplexSeg &s2)
{ return dist > s2.dist; }

SimplexSeg * SimplexSeg::cut(Point2D &ptA3, Point2D &ptB3, Point2D &ptC3)
{
	SimplexSeg *res = new SimplexSeg(ptA1, ptB1, ptC1, ptA3, ptB3, ptC3);
	*this = SimplexSeg(ptA2, ptB2, ptC2, ptA3, ptB3, ptC3);	  
	return res;
}

Vector2D SimplexSeg::getSupportVect()
{ return v; }

void SimplexSeg::getABpoints(Point2D *a, Point2D *b)
{
	Vector2D abA(ptA1, ptA2);
	*a = ptA1 + (abA * bCoord);

	Vector2D abB(ptB1, ptB2);
	*b = ptB1 + (abB * bCoord);
}
#pragma endregion // SimplexSeg

#pragma region EPAsolver
EPAsolver::EPAsolver(GJKsolver &simplex) : gsolv(simplex)
{
	if(simplex.simplexSize == 1)
	{
		distnull = true;
		return;
	}
	else if(simplex.simplexSize == 2)
	{
		// transform current 1-simplex adding 2 points
		Vector2D v(simplex.ptsC[0] - simplex.ptsC[1]);
		v.ortho();
		Vector2D vflect = v.reflexion();
		Point2D pa, pb, pc;
		gsolv.A.getMarginedSupportPoint(v, &pa);							 
		gsolv.B.getMarginedSupportPoint(vflect, &pb);
				 
		Vector2D pcj = simplex.ptsC[0]; 
		Vector2D pci = simplex.ptsC[1];
		SimplexSeg *sg = new SimplexSeg(simplex.ptsA[0], simplex.ptsB[0], Point2D(pcj.getX(),pcj.getY()), simplex.ptsA[1], simplex.ptsB[1], Point2D(pci.getX(),pci.getY()));	  
		SimplexSeg *sg2 = sg->cut(pa, pb, pa - pb);
		if(sg->isValid())
		{  
			sd.push(sg);
			if(sg->getdist() < FLT_EPSILON)
			{
				distnull = true;
				return;
			}
		}
		else
			delete sg;
		if(sg2->isValid())
		{		
			sd.push(sg2);
			if(sg->getdist() < FLT_EPSILON)
			{
				distnull = true;
				return;
			}
		}
		else
			delete sg2;

		gsolv.A.getMarginedSupportPoint(vflect, &pa);							 
		gsolv.B.getMarginedSupportPoint(v, &pb);
		sg = new SimplexSeg(simplex.ptsA[0], simplex.ptsB[0], Point2D(pcj.getX(),pcj.getY()), simplex.ptsA[1], simplex.ptsB[1], Point2D(pci.getX(),pci.getY()));	  
		sg2 = sg->cut(pa, pb, pa - pb);
		if(sg->isValid())
		{
			if(sg->getdist() < FLT_EPSILON)
			{
				distnull = true;
				return;
			}
			sd.push(sg);
		}
		else
			delete sg;
		if(sg2->isValid())
		{
			if(sg->getdist() < FLT_EPSILON)
			{
				distnull = true;
				return;
			}
			sd.push(sg2);
		}
		else
			delete sg2;
	}
	else // == 3
	{
		int j = 2;
		Point2D pcj;
		for(int i=0; i < 3; i++)
		{
			Vector2D pcj = simplex.ptsC[j]; 
			Vector2D pci = simplex.ptsC[i];
			SimplexSeg *sg = new SimplexSeg(simplex.ptsA[j], simplex.ptsB[j], Point2D(pcj.getX(),pcj.getY()), simplex.ptsA[i], simplex.ptsB[i], Point2D(pci.getX(),pci.getY()));
			if(sg->isValid())
				sd.push(sg);
			else
				delete sg;
			j = i;
		}
	}
	distnull = false;
}

Vector2D EPAsolver::getPenetrationDepth(Point2D *pA, Point2D *pB)
{
	Vector2D p(0 ,0);

	if(distnull)
	{
		if(gsolv.simplexSize == 1)
		{
			*pA = gsolv.ptsA[0];  
			*pB = gsolv.ptsB[0];
		}
		else
		{
			SimplexSeg * sg;
			sg = sd.top();
			sg->getABpoints(pA, pB);
		}
	}
	else
	{	
		SimplexSeg * sg;
		float stopHull = FLT_MAX;
		Point2D lastpc1, lastpc2;
		int loop = 0;
		do
		{
			loop++;
			sg = sd.top();
			sd.pop();
			float dst = sg->getdist();
			p = sg->getSupportVect();
			Point2D pa, pb, pc;
			Vector2D np = p.direction();
			gsolv.A.getMarginedSupportPoint(np, &pa);							 
			gsolv.B.getMarginedSupportPoint(np.reflexion(), &pb);
			pc = pa - pb;
			if(pc.equals(lastpc1) || pc.equals(lastpc2) || loop>20)
				break; // EPA wont converge due to imprecision (will always return the same support point). So exit with current approximation!
			lastpc1 = lastpc2;
			lastpc2 = pc;
			Vector2D ab(pc);
			float dot = (ab * p);
			stopHull = min(stopHull, dot * dot / dst);
			if(stopHull <= EPSILON_1 * dst)
				break;
			SimplexSeg *sg2 = sg->cut(pa, pb, pa - pb);
			if(sg->isValid())
				sd.push(sg);
			else
				delete sg;
			if(sg2->isValid())
				sd.push(sg2);
			else
				delete sg2;
		if(sd.empty())
			printf("fail");
		}while(!sd.empty());
		sg->getABpoints(pA, pB);
	}
	// free memory
	int i = sd.size() - 1;
	while(i >= 0)
	{
		SimplexSeg * d = sd.top();
		sd.pop();
		delete d;
		i--;
	}
	// end free memory
	return p;
}
#pragma endregion  // EPAsolver