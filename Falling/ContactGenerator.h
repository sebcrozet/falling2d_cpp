#ifndef CGEN
#include "CollisionArbiter.h"
#include "RigidBody.h"
#include <vector>

struct Contact
{
	Shape *s1;
	Shape *s2;
	float penetration;
	Point2D absoluteContactPoint;
	Vector2D relContactPoint[2];
	Vector2D normal, tangeant;
	Vector2D closingVelocity;
	float desiredVelocityChange;
	float totalInertia;
	float dvel,dvely;
	float lin1,lin2;

	inline Vector2D toLocal(Vector2D &v);
	inline Vector2D toGlobal(Vector2D &v);
	void updateVelChange(float dt);
	void awakeIfNeeded();
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
};
#define CGEN
#endif