#include "stdafx.h"
#include "VitessSolver.h"

void VitessSolver::integrate(std::vector<RigidBody*> r,Real dt)
{
	for(unsigned int i=0;i<r.size();i++)
	{
		RigidBody * rb = r[i];
		if(rb->isSleeping()) continue;
		if(!rb->getShape()->isFixed())
		{
			//rb->updateSleepState(dt); 
			//rb->getV().print();
			rb->multV(pow(0.995,dt));
			rb->multO(pow(0.995,dt));
			rb->addV(Vector2D(0,G*dt,0));
			rb->setPos(rb->getPos()+rb->getV()*PIX_PER_METTER*dt);
			rb->setDeltaTeta(-PIX_PER_METTER*rb->getOmega()*dt);
			rb->setTeta(rb->getTeta()+rb->getDeltaTeta());
			rb->multV(pow(0.995,dt));
			rb->multO(pow(0.995,dt));
		}
	}
}
