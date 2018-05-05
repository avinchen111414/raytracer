#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "geometricobject.h"
#include "utilities/point3d.h"
#include "utilities/normal.h"

class Triangle: public GeometricObject
{
public:
	Triangle(const Point3D& _v0, const Point3D& _v1,
		const Point3D& _v2);
	Triangle(const Triangle& other);
	Triangle& operator= (const Triangle& rhs);
	~Triangle();

	virtual GeometricObject* clone() const;
	virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool shadow_hit(const Ray& ray, float& tmin) const;

private:
	Point3D v0, v1, v2;
	Normal normal;
};

#endif