#ifndef __BOX_H__
#define __BOX_H__

#include "geometricobject.h"
#include "utilities/bbox.h"

class Box: public GeometricObject
{
public:
	Box(float _x0, float _y0, float _z0,
		float _x1, float _y1, float _z1);
	Box(const Point3D& p0, const Point3D& p1);
	Box(const Box& other);
	Box& operator= (const Box& rhs);
	~Box();

	virtual GeometricObject* clone() const;
	virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
	virtual bool shadow_hit(const Ray& ray, float& tmin) const;
	virtual const BBox* get_bounding_box() const;

protected:
	Normal get_normal(int face_hit) const;

private:
	float x0, y0, z0, x1, y1, z1;
	BBox m_bbox;
	static const float kEpsilon;
};

#endif