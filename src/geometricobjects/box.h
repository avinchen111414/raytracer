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

	virtual GeometricObject* Clone() const;
	virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;
	virtual const BBox* GetBoundingBox() const;

protected:
	Normal GetNormal(int face_hit) const;

private:
	float m_x0, m_y0, m_z0, m_x1, m_y1, m_z1;
	BBox m_bbox;
	static const float kEpsilon;
};

#endif