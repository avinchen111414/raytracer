#include "triangle.h"
#include "utilities/utility.h"
#include "utilities/ray.h"
#include "utilities/shaderec.h"

Triangle::Triangle(const Point3D& _v0, const Point3D& _v1, const Point3D& _v2)
	: GeometricObject(), v0(_v0), v1(_v1), v2(_v2)
{
	normal = (v1 - v0) ^ (v2 - v0);
	normal.normalize();
}

Triangle::Triangle(const Triangle& other)
	: GeometricObject(other), 
	v0(other.v0), v1(other.v1), v2(other.v2), normal(other.normal)
{

}

Triangle& Triangle::operator= (const Triangle& rhs)
{
	if (this == &rhs)
		return *this;
	
	GeometricObject::operator=(rhs);
	v0 = rhs.v0; v1 = rhs.v1; v2 = rhs.v2;
	normal = rhs.normal;

	return *this;
}

Triangle::~Triangle()
{

};

GeometricObject* Triangle::clone() const
{
	return new Triangle(*this);
}

bool Triangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x; 
	double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;

	double inv_denom  = 1.0 / (a * m + b * q + c * s);

	// beta =	|d b c|				D = |a b c|
	//			|h f g| D_inv			|e f g|
	//			|l j k|					|i j k|

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
		return (false);

	double r = r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0 )
		return (false);

	if (beta + gamma > 1.0)
		return (false);

	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;

	if (t < kEpsilon) 
		return (false);

	tmin 				= t;
	sr.normal 			= normal;  	
	sr.local_hit_point 	= ray.o + t * ray.d;	

	return (true);	
}

bool Triangle::shadow_hit(const Ray& ray, float& tmin) const
{
	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x; 
	double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;

	double inv_denom  = 1.0 / (a * m + b * q + c * s);

	// beta =	|d b c|				D = |a b c|
	//			|h f g| D_inv			|e f g|
	//			|l j k|					|i j k|

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
		return (false);

	double r = r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0 )
		return (false);

	if (beta + gamma > 1.0)
		return (false);

	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;

	if (t < kEpsilon) 
		return (false);

	tmin 				= t;

	return (true);	
}
