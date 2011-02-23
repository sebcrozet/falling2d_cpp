#ifndef RBODY
#include "TunningConstants.h"
#include "Polygon2D.h"
#include "Disk.h"
#define SLEEPLIMIT 2.f
#define BIAS 0.6f
#define NBRLOOPSTABILISATION 50
class FALLINGAPI RigidBody
{
private:
	float omega;
	Vector2D v;
	float invM;
	float invI;
	Vector2D acc;
	// sleep datas
	bool sleeping;
	float movment;
	//
	// position stabilisation detector
	int nbrValues;
	float tetas[NBRLOOPSTABILISATION];
	Vector2D poss[NBRLOOPSTABILISATION];
	float lastTeta;
	Vector2D lastPos;	 
	Vector2D totalDPos;
	float totalDTeta;
	int loopCursor;
	//
	float dTeta;
	float consumedDeltaTeta;

	Shape *s;

	RigidBody(Shape *s,float m,Vector2D pos,float teta = 0);   
	void reinitStabilisationDetector();
public:
	//Do not export these function!
	inline Shape *getShape();
	void updateSleepState(float dt);
	bool updateMovementStabilisationState();
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
	inline void setMWithDensity(float rho);
	inline float getI();  
	inline float getInvI();
	inline float getA();
	inline void setDeltaTeta(float dt)
	{
		consumedDeltaTeta = 0.f; // reinit consumed rotation (for penetration resolution)
		dTeta = dt; 
	}							  
	inline void addConsumedTeta(float dt)
	{ 
		consumedDeltaTeta += dt;
		// TODO :  remove validity test
		if(consumedDeltaTeta*dTeta < 0
			|| abs(consumedDeltaTeta) > abs(dTeta))
			dTeta = dTeta;
		//
	}
	inline float getConsumedDeltaTeta()
	{ return consumedDeltaTeta; }
	inline float getDeltaTeta()
	{ return dTeta; }
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
{ return Vector2D(0.f,2.f*196.2f,0.f)/*/acc/*/; }

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
{ invM = 1.0f / mass; invI = 1.0f / s->getInertiaMomentum(mass / s->getSurface()); }
inline void RigidBody::setMWithDensity(float density)
{ invM = 1.0f / (density * s->getSurface()); invI = 1.0f / s->getInertiaMomentum(density); }

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