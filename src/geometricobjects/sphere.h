#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "geometricobject.h"
#include "utilities/point3d.h"

class Sphere: public GeometricObject
{
public:
	Sphere(void);
	Sphere(const Point3D& center, const float radius);

	virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
	virtual bool shadow_hit(const Ray& ray, float& tmin) const;
	virtual GeometricObject* clone() const;
public:
	Point3D m_center;
	float m_radius;
	static const double m_epsilon;
};

#endif