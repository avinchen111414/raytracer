#include <cmath>
#include "perfectspecular.h"
#include "utilities/shaderec.h"

PerfectSpecular::PerfectSpecular()
	: BRDF(), m_kr(1), m_cr(1)
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
	m_kr = rhs.m_kr;
	m_cr = rhs.m_cr;

	return *this;
}

PerfectSpecular::~PerfectSpecular()
{

}

PerfectSpecular* PerfectSpecular::Clone() const
{
	return new PerfectSpecular(*this);
}

RGBColor PerfectSpecular::SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const
{
	float ndotwo = sr.normal * wo;
	wi = -wo + 2 * ndotwo * sr.normal;
	// sr.normal * wi 是为了抵消渲染方程中的cos项
	return m_kr * m_cr / fabs(sr.normal * wi);	
}

RGBColor PerfectSpecular::SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
	float ndotwo = sr.normal * wo;
	wi = -wo + 2.0 * sr.normal * ndotwo; 
	pdf = sr.normal * wi;
	return (m_kr * m_cr);
}