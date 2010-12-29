#include "stdafx.h"
#include "VitessSolver.h"

void VitessSolver::integrate(std::vector<RigidBody*> r,float dt)
{
	for(int i=0;i<r.size();i++)
	{
		RigidBody * rb = r[i];
		if(!rb->getShape()->isFixed())
		{
			rb->getV().print();
			rb->addV(Vector2D(0,18.81,0)*dt);
			rb->setPos(rb->getPos()+rb->getV()*dt);
			//rb->setTeta(rb->getTeta()-rb->getOmega()*dt);
		}
	}
}