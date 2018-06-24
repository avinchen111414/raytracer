#include "utilities/ray.h"
#include "sphere.h"
#include "utilities/shaderec.h"
#include <math.h>

const double Sphere::m_epsilon = 0.001;

Sphere::Sphere(void)
	: m_center(0.0), m_radius(1.0)
{
	update_bbox();
}

Sphere::Sphere(const Point3D& center, const float radius)
	: m_center(center), m_radius(radius)
{
	update_bbox();
}

bool Sphere::hit(const Ray& ray, double& tmin, ShadeRec& sr)const
{
	// (d*d)t^2 + [2(o-c) *d]t + (o-c)(o-c) - r^2
	Vector3D temp = ray.o - m_center;
	double a = (ray.d * ray.d);
	double b = 2 * (temp * ray.d);
	double c = temp * temp - m_radius * m_radius;
	double discision = b * b - 4 * a * c;
	if (discision <= 0)
	{
		return false;
	}
	else {
		double e = sqrt(discision);
		double denom = 2.0 * a;
		double root = (-b - e) / denom;
		if (root > m_eplison)
		{
			tmin = root;
			sr.normal = (temp + root * ray.d) / m_radius;
			sr.local_hit_point = ray.o + root * ray.d;
			return true;
		}

		root = (-b + e) * denom;
		if (root > m_eplison)
		{
			tmin = root;
			sr.normal = (temp + root * ray.d) / m_radius;
			sr.local_hit_point = ray.o + root * ray.d;
			return true;
		}
	}
	return false;
}

bool Sphere::shadow_hit(const Ray& ray, float& tmin) const 
{
	if (!m_shadow)
		return false;

	Vector3D temp = ray.o - m_center;
	double a = (ray.d * ray.d);
	double b = 2 * (temp * ray.d);
	double c = temp * temp - m_radius * m_radius;
	double discision = b * b - 4 * a * c;
	if (discision <= 0)
	{
		return false;
	}
	else {
		double e = sqrt(discision);
		double denom = 2.0 * a;
		double root = (-b - e) / denom;
		if (root > m_eplison)
		{
			tmin = (float)root;
			return true;
		}

		root = (-b + e) * denom;
		if (root > m_eplison)
		{
			tmin = (float)root;
			return true;
		}
	}
	return false;
}

GeometricObject* Sphere::clone() const
{
	return (new Sphere(*this));
}

const BBox* Sphere::get_bounding_box() const
{
	return &m_bbox;
}

void Sphere::update_bbox()
{
	m_bbox.x0 = m_center.x - m_radius;
	m_bbox.y0 = m_center.y - m_radius;
	m_bbox.z0 = m_center.z - m_radius;
	m_bbox.x1 = m_center.x + m_radius;
	m_bbox.y1 = m_center.y + m_radius;
	m_bbox.z1 = m_center.z + m_radius;
}
