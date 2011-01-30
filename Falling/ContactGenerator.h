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
	float penetration;
public:
	Point2D absoluteContactPoint;
	Vector2D relContactPoint[2];
	Vector2D normal, tangeant;
	Vector2D closingVelocity;
	float desiredVelocityChange;
	float totalInertia;
	float dvel,dvely;
	float lin1,lin2;
	float angin[2];
	float linin[2];
	float unitangmov[2];
	float unitlinmov[2];

	inline Vector2D toLocal(Vector2D &v);
	inline Vector2D toGlobal(Vector2D &v);
	void updateVelChange(float dt);
	void awakeIfNeeded();
	// TODO: for debug
	inline void setPenetration(float p) 
	{
		penetration = p;
		if(!(penetration == penetration))
			penetration = 0;
	}
	inline float getPenetration()
	{
		return penetration;
	}
	//
};
inline Vector2D Contact::toLocal(Vector2D &v)
{ return Vector2D(v.dot(normal),v.dot(tangeant)); }
inline Vector2D Contact::toGlobal(Vector2D &v)
{ return Vector2D(v.getX()*normal.getX()+v.getY()*tangeant.getX(),v.getX()*normal.getY()+v.getY()*tangeant.getY()); }

class ContactGenerator
{
private:
public:
	static void DeduceContactsDatas(std::vector<Collision *> &collisions, std::vector<Contact *> &concacts,float dt);
	static void DeduceContactsDatasOldAlgorithm(std::vector<Collision *> &collisions, std::vector<Contact *> &concacts,float dt);
};
#define CGEN
#endif