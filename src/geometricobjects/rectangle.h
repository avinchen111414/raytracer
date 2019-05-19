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

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;
	virtual GeometricObject* Clone() const;

	virtual Point3D Sample();
	virtual float Pdf(const ShadeRec& sr);
	virtual Normal GetNormal(const Point3D& p) const;

	void SetSampler(Sampler* sampler);


private:
	Point3D m_p0;	// left-down corner vertex
	Vector3D m_a; // side
	Vector3D m_b; // side
	double m_a_len_squared; // square of the length of side a
	double m_b_len_squared; // square of the length of side b
	Normal m_normal;

	// for area light
	float m_area;
	float m_inv_area;
	Sampler* m_sampler_ptr;

	static const double kEpsilon;

};


}

#endif