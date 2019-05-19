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

	virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;
	virtual GeometricObject* Clone() const;
	virtual const BBox* GetBoundingBox() const;

protected:
	void UpdateBbox();

public:
	Point3D m_center;
	float m_radius;
	static const double m_epsilon;

private:
	BBox m_bbox;
};

#endif