#include "stdafx.h"
#include "ContactGenerator.h"

bool QuarterSpace::compare(QuarterSpace *d1, QuarterSpace *d2)
{
  if(d1->v.getY() > 0)
    {
      if(d2->v.getY() > 0)
        return d1->v.getX() < d2->v.getX();
      else
        return false; // d2 >
    }
  if(d2->v.getY() < 0)
    return d1->v.getX() > d2->v.getX();
  else
    return true;
}

bool QuarterSpace::getSignificantDualQuarterSpace(QuarterSpace *q[4])
{
  QuarterSpace *tmp;
  // insersion sort
  for(int i = 1; i < 4; i++) // begin at the second element
    {
      tmp = q[i];
      int j = i;
      for(; j > 0; j--)
        {
          if(compare(q[j-1],tmp))
            q[j] = q[j-1];
          else
            break;
        }
      q[j] = tmp;
    }
  // It's sorted in increasing order now (on the trigonometric circle, in counterclockwise)...
  int order = 0;
  for(int i = 0,j = 3; i < 4; j=i,i++)
    {
      if(q[j]->l)
        q[j]->l = q[i]->r;
      else
        q[i]->r = false;
    }
  for(int i = 0; i<4; i++)
    {
      tmp = q[i];
      if(tmp->l || tmp->r) // at least one non-blocking half-space
        {
          if(order < 2)
            {
              q[i] = q[order];
              q[order] = tmp;
            }
          else // no liberty
            return false;
          order++;
        }
    }
  return true; // TODO: virify this return value...
}


////

void Contact::updateVelChange(Real t)
{
  Real vFromAcc = s1->getParent()->getAcc() * t * normal;
  if(s2)
    vFromAcc -= s2->getParent()->getAcc() * t * normal;
  Real fakerest = 0.4;
  if(ABS(closingVelocity.getX()) < 0.1)
    {
      fakerest = 0;
    }
  desiredVelocityChange = -closingVelocity.getX() - fakerest /* restitution */ * (closingVelocity.getX() - vFromAcc);
}

void Contact::awakeIfNeeded()
{
  if(!s2) return;
  bool s1sl = s1->getParent()->isSleeping();
  bool s2sl = s2->getParent()->isSleeping();
  if(s1sl || s2sl)
    {
      if(s1sl) // s2 not sleeping
        s1->getParent()->setAwake(true);
      if(s2sl)
        s2->getParent()->setAwake(true);
    }
}


void ContactGenerator::DeduceContactsDatas(std::vector<Collision *> &collisions, std::vector<Contact *> &cts,Real dt)
{
  for(unsigned int i=0; i<collisions.size(); i++)
    {
      Collision *c = collisions[i];
      if(c->sa->getParent()->isSleeping() && c->sb->getParent()->isSleeping())
        continue; // do not overwrite cached datas
      int max = c->c.size();
      c->worstContact = 0;
      c->worstPenetrationAmount = 0.01;
      if(max)
        {
          c->cnts = new Contact *[max];
          for(int j=0; j<max; j++)
            {
              // TODO do not make a copy of the SubCollision structure!
              // end TODO
              Contact *cnt = new Contact();
              Point2D absA = c->sa->toGlobal(c->c[j]->relPtA);
              Point2D absB = c->sb->toGlobal(c->c[j]->relPtB);
              cnt->s1 = c->sa;
              cnt->s2 = c->sb;
              cnt->absoluteContactPoint= Point2D::getMiddle(absA, absB);
              cnt->normal = c->c[j]->normal;
              cnt->setPenetration(c->c[j]->depth - 2.0 * PROXIMITY_AWARENESS); // normalise normal and return penetration depth
              if(cnt->s1->isFixed())// || */cnt->s1->getStackLevel() < cnt->s2->getStackLevel())
                {
                  /*if(!cnt->s1->isFixed())
                  cnt->s2 = cnt->s1;
                  else
                  cnt->s2 = 0;
                  */
                  cnt->s1 = cnt->s2;
                  cnt->s2 = 0;
                  cnt->normal.reflect();
                }
              else if(cnt->s2->isFixed())//|| */cnt->s1->getStackLevel() > cnt->s2->getStackLevel())
                {
                  /*
                    if(!cnt->s2->isFixed())
                  cnt->s2 = cnt->s2;
                    else
                  cnt->s2 = 0;
                  */
                  cnt->s2 = 0;
                }
              /*
              else
                  cnt->s2 = cnt->s2;
              	*/
              // get tangeant
              cnt->tangeant = Vector2D(-cnt->normal.getY(),cnt->normal.getX());
              // now calculate closing velocity
              cnt->relContactPoint[0] = cnt->s1->toTranslatedInv(cnt->absoluteContactPoint);
              if(cnt->s2)
                cnt->relContactPoint[1] =
                  cnt->s2->toTranslatedInv(cnt->absoluteContactPoint);
              RigidBody *ra = cnt->s1->getParent();
              Vector2D lin1 =
                cnt->toLocal(Vector2D(0,0,ra->getOmega()).cross(cnt->relContactPoint[0])
                             + ra->getV());
              Vector2D lin2;
              RigidBody *rb = (cnt->s2?cnt->s2->getParent():0);
              cnt->lin1 = lin1.magnitude();
              cnt->lin2 = 0;
              if(rb)
                {
                  lin2 = cnt->toLocal(
                           Vector2D(0,0,rb->getOmega()).cross(cnt->relContactPoint[1])
                           + rb->getV());
                  cnt->lin2 = lin2.magnitude();
                  cnt->closingVelocity = lin1 - lin2;
                }
              else if(cnt->s2)
                {
                  Real lin2nfixed = (cnt->toLocal(Vector2D(0,0,cnt->s2->getParent()->getOmega()).cross(cnt->relContactPoint[1]) + cnt->s2->getParent()->getV())).magnitude();
                  cnt->closingVelocity = lin1 - lin2nfixed;
                }
              else cnt->closingVelocity = lin1;
              cnt->updateVelChange(dt);
              Vector2D dvel = ((cnt->relContactPoint[0] ^ cnt->normal) * ra->getInvI())^cnt->relContactPoint[0];
              cnt->angin[0] = dvel * cnt->normal;
              cnt->linin[0] = ra->getInvM();
              cnt->dvel = cnt->angin[0] + cnt->linin[0];
              Vector2D dvely = ((cnt->relContactPoint[0] ^ cnt->tangeant) * ra->getInvI())^cnt->relContactPoint[0];
              cnt->dvely = dvely * cnt->tangeant + ra->getInvM();
              if(cnt->s2)
                {
                  dvel = ((cnt->relContactPoint[1] ^ cnt->normal) * cnt->s2->getParent()->getInvI())^cnt->relContactPoint[1];
                  cnt->angin[1] = dvel * cnt->normal;
                  cnt->linin[1] = cnt->s2->getParent()->getInvM();
                  cnt->dvel += cnt->angin[1] + cnt->linin[1];
                  dvely = ((cnt->relContactPoint[1] ^ cnt->tangeant) * cnt->s2->getParent()->getInvI())^cnt->relContactPoint[1];
                  cnt->dvely += dvely * cnt->tangeant + cnt->s2->getParent()->getInvM();
                }
              cnt->totalInertia = 1.0 / (cnt->angin[0] + cnt->linin[0] + (rb?cnt->angin[1] + cnt->linin[1] :0.0));
              cnt->unitlinmov[0] = cnt->linin[0] * cnt->totalInertia;
              cnt->unitangmov[0] = (((cnt->relContactPoint[0] ^ cnt->normal) * ra->getInvI()).getZ())*cnt->totalInertia;
              if(rb)
                {
                  cnt->unitlinmov[1] = cnt->linin[1] * cnt->totalInertia;
                  cnt->unitangmov[1] = (((cnt->relContactPoint[1] ^ cnt->normal) * rb->getInvI()).getZ())*cnt->totalInertia;
                }
              cts.push_back(cnt);
              c->cnts[j] = cnt;
              if(cnt->getPenetration() > c->worstPenetrationAmount)
                {
                  c->worstPenetrationAmount = cnt->getPenetration();
                  c->worstContact = cnt;
                }
            }
          // Determine liberties' degrees
          /*
             QuarterSpace *liberties[4];
             QuarterSpace *sp = new QuarterSpace();
             sp->v = c->cnts[0]->tangeant;
             sp->r = true;
             sp->l = false;
             liberties[0] = sp;
             sp = new QuarterSpace();
             sp->v = sp->v.reflexion();
             sp->r = false;
             sp->l = true;
             liberties[1] = sp;
             for(int j = 1; j < max; j++)
             {
             QuarterSpace *sp = new QuarterSpace();
             sp->v = c->cnts[j]->tangeant;
             sp->r = true;
             sp->l = false;
             liberties[2] = sp;
             sp = new QuarterSpace();
             sp->v = sp->v.reflexion();
             sp->r = false;
             sp->l = true;
             liberties[3] = sp;
             if(QuarterSpace::getSignificantDualQuarterSpace(liberties))
             {
             delete liberties[2];
             delete liberties[3];
             c->liberty1 = liberties[0];
             c->liberty2 = liberties[1];
             }
             else
             {
          // no liberties
          for(int k=0;k<4;k++)
          delete liberties[k];
          c->liberty1 = 0;
          // setsing liberty2 at 0 is useless
          }

          }
          */
        }
      else
        c->cnts = 0;
    }
}

