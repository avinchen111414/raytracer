#include "lambertian.h"
#include "utilities/shaderec.h"
#include "utilities/vector3d.h"
#include "utilities/utility.h"
#include "sampler/sampler.h"

Lambertian::Lambertian():
	BRDF(),
	m_kd(0),
	m_cd(0)
{}

Lambertian::Lambertian(const Lambertian& lamb):
	BRDF(lamb),
	m_kd(lamb.m_kd),
	m_cd(lamb.m_cd)
{}

Lambertian& Lambertian::operator=(const Lambertian& rhs)
{
	if (this == &rhs)
		return *this;
	BRDF::operator= (rhs);

	m_kd = rhs.m_kd;
	m_cd = rhs.m_cd;

	return *this;
}

Lambertian::~Lambertian()
{

}

Lambertian* Lambertian::Clone() const
{
	return new Lambertian(*this);
}

RGBColor Lambertian::F(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	return m_kd * m_cd * (float)invPI;
}

RGBColor Lambertian::SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const
{
	return m_kd * m_cd * (float)invPI;
}

RGBColor Lambertian::SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
	Vector3D w = sr.normal;
	Vector3D v = Vector3D(0.0034, 1, 0.0071) ^ w;
	v.Normalize();
	Vector3D u = v ^ w;

	Point3D sp = m_sampler_ptr->sample_hemisphere();  
	wi = sp.x * u + sp.y * v + sp.z * w;
	wi.Normalize(); 	

	pdf = sr.normal * wi * invPI;

	return (m_kd * m_cd * invPI);
}

RGBColor Lambertian::Rho(const ShadeRec& sr, const Vector3D& wo) const
{
	return m_kd * m_cd;
}