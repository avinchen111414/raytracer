#ifndef __PLANE_H__
#define __PLANE_H__

#include "utilities/point3d.h"
#include "utilities/normal.h"
#include "geometricobject.h"

class Plane: public GeometricObject
{
public:
	Plane(void);
	Plane(const Point3D& point, const Normal& normal);
	virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
	virtual bool shadow_hit(const Ray& ray, float& tmin) const;
	virtual GeometricObject* clone() const;

private:
	Point3D m_point;
	Normal m_normal;
	static const double m_epsilon;
};

#endif