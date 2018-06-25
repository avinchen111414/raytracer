#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "geometricobject.h"
#include "utilities/point3d.h"
#include "utilities/bbox.h"

class Sphere: public GeometricObject
{
public:
	Sphere(void);
	Sphere(const Point3D& center, const float radius);

	virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
	virtual bool shadow_hit(const Ray& ray, float& tmin) const;
	virtual GeometricObject* clone() const;
	virtual const BBox* get_bounding_box() const;

protected:
	void update_bbox();

public:
	Point3D m_center;
	float m_radius;
	static const double m_epsilon;

private:
	BBox m_bbox;
};

#endif