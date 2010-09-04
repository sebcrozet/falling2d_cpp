#ifndef GHELP
#include "Primitives.h"


namespace GeometryHelper
{
   class Transformation2D
   {
   private:
	   Vector2D u;
	   float teta, coteta, siteta;

   public:										   
	   Transformation2D();
	   Transformation2D(const Transformation2D &t); 
	   Transformation2D(Vector2D &u, float teta);

	   inline void setTeta(float t);
	   inline void addTeta(float t);
	   inline float getTeta();
	   inline void setU(Vector2D &nu);
	   inline void addU(Vector2D &nu);
	   inline Vector2D getU();

	   inline Vector2D transform(Vector2D &x);	 
	   inline Point2D transform(Point2D &x);   
	   inline Vector2D transforminv(Vector2D &x);	 
	   inline Point2D transforminv(Point2D &x);

	   inline Vector2D rotate(Vector2D &x);	 
	   inline Point2D rotate(Point2D &x);  
	   inline Vector2D rotateinv(Vector2D &x);	

	   inline Vector2D translate(Vector2D &x);	 
	   inline Point2D translate(Point2D &x);
   };
}
inline void GeometryHelper::Transformation2D::setTeta(float t)
{ teta = t; coteta = cos(t); siteta = sin(t); }
inline void GeometryHelper::Transformation2D::addTeta(float t)
{ setTeta(teta + t); }
inline float GeometryHelper::Transformation2D::getTeta()
{ return teta; }
inline void GeometryHelper::Transformation2D::setU(Vector2D &v)
{ u = v; }
inline void GeometryHelper::Transformation2D::addU(Vector2D &v)
{ u += v; }
inline Vector2D GeometryHelper::Transformation2D::getU()
{ return u; }

inline Vector2D GeometryHelper::Transformation2D::translate(Vector2D &x)
{ return x + u; }
inline Point2D GeometryHelper::Transformation2D::translate(Point2D &x)
{ return x + u; }	   
inline Vector2D GeometryHelper::Transformation2D::rotate(Vector2D &x)
{
	float X = x.getX(), Y = x.getY();
	return Vector2D(X * coteta + Y * siteta, Y * coteta - X * siteta);
}
inline Point2D GeometryHelper::Transformation2D::rotate(Point2D &x)
{ 
	float X = x.getX(), Y = x.getY();
	return Point2D(X * coteta + Y * siteta, Y * coteta - X * siteta);
}	   
inline Vector2D GeometryHelper::Transformation2D::rotateinv(Vector2D &x)
{
	float X = x.getX(), Y = x.getY();
	return Vector2D(X * coteta - Y * siteta, Y * coteta + X * siteta);
}
inline Vector2D GeometryHelper::Transformation2D::transform(Vector2D &x)
{
	float X = x.getX(), Y = x.getY();
	return Vector2D(X * coteta + Y * siteta + u.getX(), Y * coteta - X * siteta  + u.getY());
}
inline Point2D GeometryHelper::Transformation2D::transform(Point2D &x)
{ 
	float X = x.getX(), Y = x.getY();
	return Point2D(X * coteta + Y * siteta + u.getX(), Y * coteta - X * siteta  + u.getY());
}   
inline Vector2D GeometryHelper::Transformation2D::transforminv(Vector2D &x)
{
	float X = x.getX(), Y = x.getY();
	return Vector2D(X * coteta - Y * siteta - u.getX(), Y * coteta + X * siteta  - u.getY());
}
inline Point2D GeometryHelper::Transformation2D::transforminv(Point2D &x)
{ 
	float X = x.getX(), Y = x.getY();
	return Point2D(X * coteta - Y * siteta - u.getX(), Y * coteta + X * siteta  - u.getY());
}
#define GHELP
#endif

            