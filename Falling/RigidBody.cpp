#include "stdafx.h"
#include "RigidBody.h"

RigidBody::RigidBody(Shape *sh,float m,Vector2D pos,float teta)
{
	sleeping = false;
	movment = SLEEPLIMIT * 2.f;
	s = sh;
	s->setParent(this);
	setTeta(teta);
	setPos(pos);
	setM(m);
	omega = 0;
}

void RigidBody::updateSleepState(float dt)
{
	float currm = v * v + omega * omega;
	float bias = pow(BIAS,dt);
	movment = (bias*movment) + (1-bias) * currm;
	if(movment > 10*SLEEPLIMIT)
		movment = 10*SLEEPLIMIT;
	else if(movment < SLEEPLIMIT)
		setAwake(false);
}

void RigidBody::setAwake(bool awake)
{
	if(awake)
	{
		sleeping = false;
		movment = SLEEPLIMIT * 2.f;
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
	Polygon2D *pp=new Polygon2D(pts,GeometryHelper::Transformation2D(pos,teta),n, fixed);
	return new RigidBody(pp,m*pp->getSurface(),pos,teta);
}

RigidBody *RigidBody::build_circularBody(Point2D &pt, float radius, bool fixed,float m,Vector2D pos,float teta)
{
	Disk * pp=new Disk(pt, radius, fixed);
	return new RigidBody(pp,m*pp->getSurface(), pos, teta);
}