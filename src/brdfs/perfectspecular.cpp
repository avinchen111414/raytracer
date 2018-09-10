#include <cmath>
#include "perfectspecular.h"
#include "utilities/shaderec.h"

PerfectSpecular::PerfectSpecular()
	: BRDF(), kr(1), cr(1)
{

}

PerfectSpecular::PerfectSpecular(const PerfectSpecular& pr)
	: BRDF(pr)
{

}

PerfectSpecular& PerfectSpecular::operator= (const PerfectSpecular& rhs)
{
	if (this == &rhs)
		return *this;

	BRDF::operator=(rhs);
	kr = rhs.kr;
	cr = rhs.cr;

	return *this;
}

PerfectSpecular::~PerfectSpecular()
{

}

PerfectSpecular* PerfectSpecular::clone() const
{
	return new PerfectSpecular(*this);
}

RGBColor PerfectSpecular::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const
{
	float ndotwo = sr.normal * wo;
	wi = -wo + 2 * ndotwo * sr.normal;
	// sr.normal * wi 是为了抵消渲染方程中的cos项
	return kr * cr / fabs(sr.normal * wi);	
}

RGBColor PerfectSpecular::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
	float ndotwo = sr.normal * wo;
	wi = -wo + 2.0 * sr.normal * ndotwo; 
	pdf = sr.normal * wi;
	return (kr * cr);
}