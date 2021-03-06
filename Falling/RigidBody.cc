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
#include "stdafx.hh"
#include "RigidBody.hh"
#include "Tesselator.hh"

namespace Falling
{
  RigidBody::RigidBody(Shape *sh,Real m,Vector2D pos,Real teta)
  {
    nbrValues = 0;
    sleeping = false;
    movment = SLEEPLIMIT * 2.0;
    s = sh;
    s->setParent(this);
    setTeta(teta);
    setPos(pos);
    setM(m);
    omega = 0;
    reinitStabilisationDetector();
  }

  RigidBody::~RigidBody()
  {
    delete s;
  }

  void RigidBody::updateSleepState(Real dt)
  {
    // detect low velocities
    Real currm = v * v + omega * omega;
    Real bias = POW(BIAS,dt);
    //bool movstab = updateMovementStabilisationState();
    movment = (bias*movment) + (1-bias) * currm;
    if(movment > 2.0*SLEEPLIMIT)
      movment = 2.0*SLEEPLIMIT;
    else if(movment < SLEEPLIMIT)
      //if(movstab)
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
    if((totalDPos * totalDPos < 4.0 && ABS(totalDTeta) < 0.01))
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
      movment = SLEEPLIMIT * 1.1;
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

  RigidBody *RigidBody::build_planarBody(
      const Point2D &origin,
      const Vector2D &normal
      )
  {
    InfinitePlane *ip = new InfinitePlane(origin, normal);
    return new RigidBody(ip, ip->getSurface(), origin);
  }

  RigidBody *RigidBody::build_polygonalBody(
      Point2D *pts,
      int n,
      bool fixed,
      Real m,
      Vector2D pos,
      Real teta
      )
  {
    Polygon2D *pp=new Polygon2D(pts, n, 0, 0, 0, Tesselator::ODD_IS_SOLID, pos, true, teta, fixed);
    return new RigidBody(pp,m*pp->getSurface(),pp->getCentroid(),teta);
  }

  RigidBody *RigidBody::build_circularBody(
      Point2D &pt,
      Real radius,
      bool fixed,
      Real m,
      Vector2D pos,
      Real teta
      )
  {
    Disk * pp=new Disk(pt, radius, fixed);
    return new RigidBody(pp,m*pp->getSurface(), pos, teta);
  }

  bool RigidBody::containsPoint(const Point2D &pt) const
  {
    return s->containsPoint(pt);
  }
}
