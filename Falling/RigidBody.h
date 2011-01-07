#ifndef RBODY
#include "Polygon2D.h"
#include "Disk.h"
#define SLEEPLIMIT 2.0f
#define BIAS 0.1f
class RigidBody
{
private:
	float omega;
	Vector2D v;
	float invM;
	float invI;
	Vector2D acc;
	bool sleeping;
	float movment;

	Shape *s;

	RigidBody(Shape *s,float m,Vector2D pos,float teta = 0);
public:
	//Do not export this function!
	inline Shape *getShape();
	void updateSleepState(float dt);
	//
	void setAwake(bool awake);
	inline bool isSleeping();

	inline Vector2D getPos();
	inline void setPos(Vector2D);
	inline float getOmega();
	inline void addRV(float rv);
	inline Vector2D getV();
	inline void addV(Vector2D &v);
	inline void multV(float d);
	inline void multO(float d);
	inline float getTeta();
	inline void setTeta(float teta);
	inline float getM();
	inline float getInvM();
	inline void setM(float mass);
	inline void setMwithDensity(float rho);
	inline float getI();
	inline float getInvI();
	inline float getA();
	inline Vector2D getAcc();

	// Rigid Body builder
	static RigidBody *build_circularBody(Point2D &pt, float radius, bool fixed,float m,Vector2D pos,float teta);
	static RigidBody *build_polygonalBody(Point2D *pts,int n, bool fixed,float m,Vector2D pos,float teta);
};
inline bool RigidBody::isSleeping()
{ return sleeping; }

inline float RigidBody::getOmega()
{ return omega; }
inline void RigidBody::addRV(float po)
{ omega += po; }
inline Vector2D RigidBody::getV()
{ return v; }
inline void RigidBody::addV(Vector2D &pv)
{ v = v + pv; }
inline void RigidBody::multV(float d)
{ v = v*d; }
inline void RigidBody::multO(float d)
{ omega = omega*d; }
inline Vector2D RigidBody::getAcc()
{ return Vector2D(0.,2*196.2,0.)/*/acc/*/; }

inline float RigidBody::getTeta()
{ return s->getTeta(); }
inline void RigidBody::setTeta(float teta)
{ s->setTeta(teta); }
inline Vector2D RigidBody::getPos()
{ return s->getPos(); }
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

inline Shape *RigidBody::getShape()
{ return s; }
#define RBODY
#endif