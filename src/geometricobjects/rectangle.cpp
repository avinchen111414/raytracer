#include "rectangle.h"
#include "sampler/sampler.h"
#include "utilities/ray.h"
#include "utilities/shaderec.h"

namespace raytracer
{
const double Rectangle::kEpsilon = 0.001;

Rectangle::Rectangle():
	GeometricObject(),
	p0(Point3D(-1, 0, -1)),
	a(Vector3D(0.0f, 0.0f, 2.0f)),
	b(Vector3D(2.0f, 0.0f, 0.0f)),
	a_len_squared(4.0f),
	b_len_squared(4.0f),
	normal(Vector3D(0, 1, 0)),
	area(4),
	inv_area(0.25f),
	sampler_ptr(nullptr)
{

}

Rectangle::Rectangle(const Point3D& _p0, const Vector3D& _a,
	const Vector3D& _b):
GeometricObject(),
	p0(_p0), a(_a), b(_b),
	a_len_squared(_a.len_squared()), b_len_squared(_b.len_squared()),
	area(_a.length() * _b.length()),
	inv_area(1.0f / (_a.length() * _b.length())),
	sampler_ptr(nullptr)
{
	normal = a ^ b;
	normal.normalize();
}

Rectangle::Rectangle(const Rectangle& other)
	:GeometricObject(other),
	p0(other.p0), a(other.a), b(other.b),
	a_len_squared(other.a_len_squared), b_len_squared(other.b_len_squared),
	area(other.area), inv_area(other.inv_area),
	normal(other.normal)
{
	if (other.sampler_ptr)
		sampler_ptr = other.sampler_ptr->clone();
	else
		sampler_ptr = nullptr;
}

GeometricObject* Rectangle::clone() const
{
	return new Rectangle(*this);
}

bool Rectangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double t = (p0 - ray.o) * normal / (ray.d * normal);
	if (t <= kEpsilon)
		return false;

	// p: hit point of ray and the surface where the rectangle lives
	Point3D p = ray.o + t * ray.d;
	Vector3D d = p - p0;

	double ddota = d * a;
	if (ddota <= kEpsilon || ddota >= a_len_squared)
		return false;

	double ddotb = d * b;
	if (ddotb <= kEpsilon || ddotb >= b_len_squared)
		return false;

	tmin = t;
	sr.normal = normal;
	sr.local_hit_point = p;

	return true;
}

bool Rectangle::shadow_hit(const Ray& ray, float& tmin) const
{
	if (!m_shadow)
		return false;

	double t = (p0 - ray.o) * normal / (ray.d * normal);
	if (t <= kEpsilon)
		return false;

	Point3D p = ray.o + t * ray.d;
	Vector3D d = p - p0;

	double ddota = d * a;
	if (ddota <= kEpsilon || ddota >= a_len_squared)
		return false;

	double ddotb = d * b;
	if (ddotb <= kEpsilon || ddotb >= b_len_squared)
		return false;

	tmin = static_cast<float>(t);
	return true;
}

Point3D Rectangle::sample()
{
	Point2D sample_point = sampler_ptr->sample_unit_square();
	return (p0 + sample_point.x * a + sample_point.y * b);
}

float Rectangle::pdf(const ShadeRec& sr)
{
	return inv_area;
}

Normal Rectangle::get_normal(const Point3D& p)
{
	return normal;
}

void Rectangle::set_sampler(Sampler* sampler)
{
	sampler_ptr = sampler;
}

}