#include "lambertian.h"
#include "utilities/shaderec.h"
#include "utilities/vector3d.h"
#include "utilities/utility.h"
#include "sampler/sampler.h"

Lambertian::Lambertian():
	BRDF(),
	kd(0),
	cd(0)
{}

Lambertian::Lambertian(const Lambertian& lamb):
	BRDF(lamb),
	kd(lamb.kd),
	cd(lamb.cd)
{}

Lambertian& Lambertian::operator=(const Lambertian& rhs)
{
	if (this == &rhs)
		return *this;
	BRDF::operator= (rhs);

	kd = rhs.kd;
	cd = rhs.cd;

	return *this;
}

Lambertian::~Lambertian()
{

}

Lambertian* Lambertian::clone() const
{
	return new Lambertian(*this);
}

RGBColor Lambertian::F(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	return kd * cd * (float)invPI;
}

RGBColor Lambertian::SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const
{
	return kd * cd * (float)invPI;
}

RGBColor Lambertian::SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
	Vector3D w = sr.normal;
	Vector3D v = Vector3D(0.0034, 1, 0.0071) ^ w;
	v.normalize();
	Vector3D u = v ^ w;

	Point3D sp = m_sampler_ptr->sample_hemisphere();  
	wi = sp.x * u + sp.y * v + sp.z * w;
	wi.normalize(); 	

	pdf = sr.normal * wi * invPI;

	return (kd * cd * invPI);
}

RGBColor Lambertian::Rho(const ShadeRec& sr, const Vector3D& wo) const
{
	return kd * cd;
}