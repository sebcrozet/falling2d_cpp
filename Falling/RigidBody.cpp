#include "stdafx.h"
#include "RigidBody.h"

RigidBody::RigidBody(Shape *sh,float m,Vector2D pos,float teta)
{
	s = sh;
	s->setParent(this);
	setTeta(teta);
	setPos(pos);
	setM(m);
}

RigidBody *RigidBody::build_polygonalBody(Point2D *pts,int n, bool fixed,float m,Vector2D pos,float teta)
{
	return new RigidBody(new Polygon2D(pts,GeometryHelper::Transformation2D(pos,teta),n, fixed),m,pos,teta);
}

RigidBody *RigidBody::build_circularBody(Point2D &pt, float radius, bool fixed,float m,Vector2D pos,float teta)
{
	return new RigidBody(new Disk(pt, radius, fixed), m, pos, teta);
}