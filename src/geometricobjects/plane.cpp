#include "utilities/ray.h"
#include "utilities/shaderec.h"
#include "plane.h"

const double Plane::m_epsilon = 0.001;

Plane::Plane(void)
	: m_point(0.0), m_normal(0.0)
{
}

Plane::Plane(const Point3D& point, const Normal& normal)
	: m_point(point), m_normal(normal)
{
}

bool Plane::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	// t = (plane_point - line_point) * normal / (line_dir * normal)
	double t = (m_point - ray.o) * m_normal / (ray.d * m_normal);
	if (t > m_epsilon)
	{
		tmin = t;
		sr.normal = m_normal;
		sr.local_hit_point = ray.o + t * ray.d;
		return true;
	}
	else
	{
		return false;
	}
}

bool Plane::shadow_hit(const Ray& ray, float& tmin) const
{
	if (!m_shadow)
		return false;

	double t = (m_point - ray.o) * m_normal / (ray.d * m_normal);
	if (t > m_epsilon)
	{
		tmin = static_cast<float>(t);
		return true;
	}
	else
	{
		return false;
	}
}

GeometricObject* Plane::clone() const
{
	return (new Plane(*this));
}