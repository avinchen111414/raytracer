#include "rectangle.h"
#include "sampler/sampler.h"
#include "utilities/ray.h"
#include "utilities/shaderec.h"

namespace raytracer
{
const double Rectangle::kEpsilon = 0.001;

Rectangle::Rectangle():
	GeometricObject(),
	m_p0(Point3D(-1, 0, -1)),
	m_a(Vector3D(0.0f, 0.0f, 2.0f)),
	m_b(Vector3D(2.0f, 0.0f, 0.0f)),
	m_a_len_squared(4.0f),
	m_b_len_squared(4.0f),
	m_normal(Vector3D(0, 1, 0)),
	m_area(4),
	m_inv_area(0.25f),
	m_sampler_ptr(nullptr)
{

}

Rectangle::Rectangle(const Point3D& _p0, const Vector3D& _a,
	const Vector3D& _b):
GeometricObject(),
	m_p0(_p0), m_a(_a), m_b(_b),
	m_a_len_squared(_a.LenSquared()), m_b_len_squared(_b.LenSquared()),
	m_area(_a.Length() * _b.Length()),
	m_inv_area(1.0f / (_a.Length() * _b.Length())),
	m_sampler_ptr(nullptr)
{
	m_normal = m_a ^ m_b;
	m_normal.Normalize();
}

Rectangle::Rectangle(const Rectangle& other)
	:GeometricObject(other),
	m_p0(other.m_p0), m_a(other.m_a), m_b(other.m_b),
	m_a_len_squared(other.m_a_len_squared), m_b_len_squared(other.m_b_len_squared),
	m_area(other.m_area), m_inv_area(other.m_inv_area),
	m_normal(other.m_normal)
{
	if (other.m_sampler_ptr)
		m_sampler_ptr = other.m_sampler_ptr->clone();
	else
		m_sampler_ptr = nullptr;
}

GeometricObject* Rectangle::Clone() const
{
	return new Rectangle(*this);
}

bool Rectangle::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double t = (m_p0 - ray.o) * m_normal / (ray.d * m_normal);
	if (t <= kEpsilon)
		return false;

	// p: hit point of ray and the surface where the rectangle lives
	Point3D p = ray.o + t * ray.d;
	Vector3D d = p - m_p0;

	double ddota = d * m_a;
	if (ddota <= kEpsilon || ddota >= m_a_len_squared)
		return false;

	double ddotb = d * m_b;
	if (ddotb <= kEpsilon || ddotb >= m_b_len_squared)
		return false;

	tmin = t;
	sr.normal = m_normal;
	sr.local_hit_point = p;

	return true;
}

bool Rectangle::ShadowHit(const Ray& ray, float& tmin) const
{
	if (!m_shadow)
		return false;

	double t = (m_p0 - ray.o) * m_normal / (ray.d * m_normal);
	if (t <= kEpsilon)
		return false;

	Point3D p = ray.o + t * ray.d;
	Vector3D d = p - m_p0;

	double ddota = d * m_a;
	if (ddota <= kEpsilon || ddota >= m_a_len_squared)
		return false;

	double ddotb = d * m_b;
	if (ddotb <= kEpsilon || ddotb >= m_b_len_squared)
		return false;

	tmin = static_cast<float>(t);
	return true;
}

Point3D Rectangle::Sample()
{
	Point2D sample_point = m_sampler_ptr->sample_unit_square();
	return (m_p0 + sample_point.x * m_a + sample_point.y * m_b);
}

float Rectangle::Pdf(const ShadeRec& sr)
{
	return m_inv_area;
}

Normal Rectangle::GetNormal(const Point3D& p) const
{
	return m_normal;
}

void Rectangle::SetSampler(Sampler* sampler)
{
	m_sampler_ptr = sampler;
}

}