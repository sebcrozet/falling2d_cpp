#include "stdafx.h"
#include "RigidBody.h"
#include "Tesselator.h"

RigidBody::RigidBody(Shape *sh,float m,Vector2D pos,float teta)
{				
	nbrValues = 0;
	sleeping = false;
	movment = SLEEPLIMIT * 2.f;
	s = sh;
	s->setParent(this);
	setTeta(teta);
	setPos(pos);
	setM(m);
	omega = 0;		
	reinitStabilisationDetector();
}

void RigidBody::updateSleepState(float dt)
{
	// detect low velocities
	float currm = v * v + omega * omega;
	float bias = pow(BIAS,dt);
	bool movstab = updateMovementStabilisationState();
	movment = (bias*movment) + (1-bias) * currm;
	if(movment > 10.f*SLEEPLIMIT)
		movment = 10.f*SLEEPLIMIT;
	/*else*/ if(movstab)
		setAwake(false);
	// TODO: detect low movments
}


// movment stabilisation manager
bool RigidBody::updateMovementStabilisationState()
{
	// algorithm running in a NBRLOOPSTABILISATION stabilisation loops
	if(nbrValues < NBRLOOPSTABILISATION)
	{				
		tetas[nbrValues] = getTeta() - lastTeta;
		poss[nbrValues] = getPos() - lastPos; 
		lastTeta = getTeta();
		lastPos = getPos();	  
		totalDPos = totalDPos + poss[nbrValues];
		totalDTeta += tetas[nbrValues];	
		nbrValues++;
		return false;
	}
	// remove one registered omega and dposs
	totalDPos = totalDPos - poss[loopCursor];
	totalDTeta -= tetas[loopCursor];
	tetas[loopCursor] = getTeta() - lastTeta;
	poss[loopCursor] = getPos() - lastPos;
	lastPos = getPos();
	lastTeta = getTeta();
	totalDPos = totalDPos + poss[loopCursor];
	totalDTeta += tetas[loopCursor];
	if(loopCursor == NBRLOOPSTABILISATION - 1)
		loopCursor = 0;
	else
		loopCursor++;
	if((totalDPos * totalDPos < 4.f && abs(totalDTeta) < 0.01))
		return true;
	return false; 
}  
void RigidBody::reinitStabilisationDetector()
{
	lastTeta = getTeta();
	lastPos = getPos();
	totalDPos.setY(0);
	totalDPos.setX(0);
	totalDTeta = 0;
	loopCursor = 0;
	nbrValues = 0;
}

void RigidBody::setAwake(bool awake)
{
	if(awake)
	{
		sleeping = false;
		movment = SLEEPLIMIT * 2.f;
		//reinitStabilisationDetector();
	}
	else
	{
		sleeping = true;
		v.setX(0);
		v.setY(0);
		omega = 0;
	}
}

RigidBody *RigidBody::build_polygonalBody(Point2D *pts,int n, bool fixed,float m,Vector2D pos,float teta)
{
	Polygon2D *pp=new Polygon2D(pts, n, 0, 0, 0, 1, pos, true, teta, 10, false, fixed);
	return new RigidBody(pp,m*pp->getSurface(),pp->getCentroid(),teta);
}

RigidBody *RigidBody::build_circularBody(Point2D &pt, float radius, bool fixed,float m,Vector2D pos,float teta)
{
	Disk * pp=new Disk(pt, radius, fixed);
	return new RigidBody(pp,m*pp->getSurface(), pos, teta);
}