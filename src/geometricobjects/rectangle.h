#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "geometricobject.h"
#include "utilities/point3d.h"
#include "utilities/vector3d.h"

class Sampler;

namespace raytracer{

class Rectangle: public GeometricObject
{
public:
	Rectangle();
	Rectangle(const Point3D& _p0, const Vector3D& _a, const Vector3D& _b);
	Rectangle(const Rectangle& other);

	virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool shadow_hit(const Ray& ray, float& tmin) const;
	virtual GeometricObject* clone() const;

	virtual Point3D sample();
	virtual float pdf(const ShadeRec& sr);
	virtual Normal get_normal(const Point3D& p) const;

	void set_sampler(Sampler* sampler);


private:
	Point3D p0;	// left-down corner vertex
	Vector3D a; // side
	Vector3D b; // side
	double a_len_squared; // square of the length of side a
	double b_len_squared; // square of the length of side b
	Normal normal;

	// for area light
	float area;
	float inv_area;
	Sampler* sampler_ptr;

	static const double kEpsilon;

};


}

#endif