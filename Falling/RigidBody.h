/* Copyright (C) 2011 CROZET Sébastien

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#ifndef RBODY
#include "TunningConstants.h"
#include "Polygon2D.h"
#include "Disk.h"
#define SLEEPLIMIT 4.0
#define BIAS 0.8
#define NBRLOOPSTABILISATION 50

namespace Falling
{
    class FALLINGAPI RigidBody
    {
	private:
	    Real omega;
	    Vector2D v;
	    Real invM;
	    Real invI;
	    Vector2D acc;
	    // sleep datas
	    bool sleeping;
	    Real movment;
	    //
	    // position stabilisation detector
	    int nbrValues;
	    Real tetas[NBRLOOPSTABILISATION];
	    Vector2D poss[NBRLOOPSTABILISATION];
	    Real lastTeta;
	    Vector2D lastPos;
	    Vector2D totalDPos;
	    Real totalDTeta;
	    int loopCursor;
	    //
	    Real dTeta;
	    Real consumedDeltaTeta;

	    Shape *s;

	    RigidBody(Shape *s,Real m,Vector2D pos,Real teta = 0);
	    void reinitStabilisationDetector();
	public:
		~RigidBody();
	    //Do not export these function!
	    inline Shape *getShape();
	    void updateSleepState(Real dt);
	    bool updateMovementStabilisationState();
	    //
	    bool containsPoint(const Point2D &pt) const;
	    void setAwake(bool awake);
	    inline bool isSleeping();

	    inline Vector2D getPos();
	    inline void setPos(Vector2D);
	    inline Real getOmega();
	    inline void addRV(Real rv);
	    inline Vector2D getV();
	    inline void addV(const Vector2D &v);
	    inline void multV(Real d);
	    inline void multO(Real d);
	    inline Real getTeta();
	    inline void setTeta(Real teta);
	    inline Real getM();
	    inline Real getInvM();
	    inline void setM(Real mass);
	    inline void setMWithDensity(Real rho);
	    inline Real getI();
	    inline Real getInvI();
	    inline Real getA();
	    inline void setDeltaTeta(Real dt)
	    {
		consumedDeltaTeta = 0.0; // reinit consumed rotation (for penetration resolution)
		dTeta = dt;
	    }
	    inline void addConsumedTeta(Real dt)
	    {
		consumedDeltaTeta += dt;
	    }
	    inline Real getConsumedDeltaTeta()
	    {
		return consumedDeltaTeta;
	    }
	    inline Real getDeltaTeta()
	    {
		return dTeta;
	    }
	    inline bool isFixed() const
	    {
		return s->isFixed();
	    }
	    inline void setFixed(bool fixed)
	    {
		s->setFixed(fixed);
	    }
	    inline Vector2D getAcc();

	    // Rigid Body builder
	    static RigidBody *build_planarBody(const Point2D &origin, const Vector2D &normal);
	    static RigidBody *build_circularBody(Point2D &pt, Real radius, bool fixed,Real m,Vector2D pos,Real teta);
	    static RigidBody *build_polygonalBody(Point2D *pts,int n, bool fixed,Real m,Vector2D pos,Real teta);
    };
    inline bool RigidBody::isSleeping()
    {
	return sleeping;
    }

    inline Real RigidBody::getOmega()
    {
	return omega;
    }
    inline void RigidBody::addRV(Real po)
    {
	omega += po;
    }
    inline Vector2D RigidBody::getV()
    {
	return v;
    }
    inline void RigidBody::addV(const Vector2D &pv)
    {
	v = v + pv;
    }
    inline void RigidBody::multV(Real d)
    {
	v = v*d;
    }
    inline void RigidBody::multO(Real d)
    {
	omega = omega*d;
    }
    inline Vector2D RigidBody::getAcc()
    {
	return Vector2D(0.0,G,0.0)/*acc*/;
    }

    inline Real RigidBody::getTeta()
    {
	return s->getTeta();
    }
    inline void RigidBody::setTeta(Real teta)
    {
	s->setTeta(teta);
    }
    inline Vector2D RigidBody::getPos()
    {
	return s->getPos();
    }
    inline void RigidBody::setPos(Vector2D pos)
    {
	s->setPos(pos);
    }

    inline Real RigidBody::getM()
    {
	return 1/invM;
    }
    inline Real RigidBody::getInvM()
    {
	return invM;
    }
    inline void RigidBody::setM(Real mass)
    {
	invM = 1.0 / mass;
	invI = 1.0 / s->getInertiaMomentum(mass / s->getSurface());
    }
    inline void RigidBody::setMWithDensity(Real density)
    {
	invM = 1.0 / (density * s->getSurface());
	invI = 1.0 / s->getInertiaMomentum(density);
    }

    inline Real RigidBody::getI()
    {
	return 1.0/invI;
    }
    inline Real RigidBody::getInvI()
    {
	return invI;
    }

    inline Real RigidBody::getA()
    {
	return s->getSurface();
    }

    inline Shape *RigidBody::getShape()
    {
	return s;
    }
}
#define RBODY
#endif
