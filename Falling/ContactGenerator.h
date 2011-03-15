#ifndef CGEN
#include "CollisionArbiter.h"
#include "RigidBody.h"
#include <vector>


struct QuarterSpace
{
	Vector2D v;
	bool r,l;
	static bool compare(QuarterSpace *d1, QuarterSpace *d2);
	// q1 and q2 will  contain the two significants quarter spaces if returns false
	static bool getSignificantDualQuarterSpace(QuarterSpace *q[4]);
};

struct Contact
{
	// TODO: see if there are no ureless datas here
	Shape *s1;
	Shape *s2;
	Shape *s2nfixed;
private:
	Real penetration;
public:
	Point2D absoluteContactPoint;
	Vector2D relContactPoint[2];
	Vector2D normal, tangeant;
	Vector2D closingVelocity;
	Real desiredVelocityChange;
	Real totalInertia;
	Real dvel,dvely;
	Real lin1,lin2;
	Real angin[2];
	Real linin[2];
	Real unitangmov[2];
	Real unitlinmov[2];

	inline Vector2D toLocal(const Vector2D &v) const;
	inline Vector2D toGlobal(const Vector2D &v) const;
	void updateVelChange(Real dt);
	void awakeIfNeeded();
	// TODO: for debug
	inline void setPenetration(Real p) 
	{
		penetration = p;
		if(!(penetration == penetration))
			penetration = 0;
	}
	inline Real getPenetration()
	{
		return penetration;
	}
	//
};
inline Vector2D Contact::toLocal(const Vector2D &v) const
{ return Vector2D(v.dot(normal),v.dot(tangeant)); }
inline Vector2D Contact::toGlobal(const Vector2D &v) const
{ return Vector2D(v.getX()*normal.getX()+v.getY()*tangeant.getX(),v.getX()*normal.getY()+v.getY()*tangeant.getY()); }

class ContactGenerator
{
private:
public:
	static void DeduceContactsDatas(std::vector<Collision *> &collisions, std::vector<Contact *> &concacts,Real dt);
	static void DeduceContactsDatasOldAlgorithm(std::vector<Collision *> &collisions, std::vector<Contact *> &concacts,Real dt);
};
#define CGEN
#endif
