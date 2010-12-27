#include "Shapes.h"

class RigidBody
{
private:
	float omega;
	Vector2D v;
	float invM;
	float invI;

	Shape *s;
public:
	inline Vector2D getPos();
	inline void setPos(Vector2D);
	inline float getOmega();
	inline Vector2D getV();
	inline float getTeta();
	inline void setTeta(float teta);
	inline float getM();
	inline float getInvM();
	inline void setM(float mass);
	inline void setMwithDensity(float rho);
	inline float getI();
	inline float getInvI();
	inline float getA();
};
inline float RigidBody::getOmega()
{ return omega; }
inline Vector2D RigidBody::getV()
{ return v; }

inline float RigidBody::getTeta()
{ s->getTeta(); }
inline void RigidBody::setTeta(float teta)
{ s->setTeta(teta); }
inline Vector2D RigidBody::getPos()
{ s->getPos(); }
inline void RigidBody::setPos(Vector2D pos)
{ s->setPos(pos); }

inline float RigidBody::getM()
{ return 1/invM; }
inline float RigidBody::getInvM()
{ return invM; }
inline void RigidBody::setM(float mass)
{ invM = 1.0f / mass; invI = 1.0f / s->getInertiaMomentum(mass); }
inline void	RigidBody::setMwithDensity(float rho)
{ setM(rho * getA()); }

inline float RigidBody::getI()
{ return 1.0f/invI; }
inline float RigidBody::getInvI()
{ return invI; }

inline float RigidBody::getA()
{ return s->getSurface(); }