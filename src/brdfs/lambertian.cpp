#include "lambertian.h"
#include "utilities/shaderec.h"
#include "utilities/vector3d.h"
#include "utilities/utility.h"

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

RGBColor Lambertian::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	return kd * cd * (float)invPI;
}

RGBColor Lambertian::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const
{
	return kd * cd * (float)invPI;
}

RGBColor Lambertian::rho(const ShadeRec& sr, const Vector3D& wo) const
{
	return kd * cd;
}