#include "stdafx.h"
#include "VitessSolver.h"

void VitessSolver::integrate(std::vector<RigidBody*> r,float dt)
{
	for(int i=0;i<r.size();i++)
	{
		RigidBody * rb = r[i];
		if(rb->isSleeping()) continue;
		if(!rb->getShape()->isFixed())
		{
			//rb->getV().print();
			rb->addV(Vector2D(0,19.62,0)*dt);
			rb->multV(0.995f);
			rb->setPos(rb->getPos()+rb->getV()*dt);
			rb->setTeta(rb->getTeta()-rb->getOmega()*dt);
		}
	}
}