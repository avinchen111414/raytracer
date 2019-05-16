#include <cmath>

#include "utilities/shaderec.h"
#include "utilities/vector3d.h"
#include "fresnelreflector.h"

FresnelReflector::FresnelReflector()
	: BRDF(), cr(1.0f)
{}

FresnelReflector::FresnelReflector(const FresnelReflector& fr)
	: BRDF(fr), cr(fr.cr)
{}

FresnelReflector::~FresnelReflector()
{}

FresnelReflector& FresnelReflector::operator=(const FresnelReflector& rhs)
{
	if (this == &rhs)
		return *this;

	BRDF::operator=(rhs);
	cr = rhs.cr;
	return *this;
}

FresnelReflector* FresnelReflector::clone() const
{
	return new FresnelReflector(*this);
}

RGBColor FresnelReflector::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const
{
	float ndotwo = sr.normal * wo;
	wi = -wo + 2 * ndotwo * sr.normal;
	return this->fresnel(sr) * cr / fabs(sr.normal * wi);
}

float FresnelReflector::fresnel(const ShadeRec& sr) const
{
	Normal normal(sr.normal);
	float ndotd = normal * -sr.ray.d;

	float eta = 0.0f;
	// Ray hits inside surface
	if (ndotd < 0.0f)
	{
		normal = -normal;
		eta = eta_out / eta_in;
	}
	else
		eta = eta_in / eta_out;

	float cos_theta_i = normal * -sr.ray.d;
	float cos_theta_t = sqrt(1.0f - (1.0f - cos_theta_i * cos_theta_i) / (eta * eta));

	float r_parallel = (eta * cos_theta_i - cos_theta_t) / 
		(eta * cos_theta_i + cos_theta_t);
	float r_perpendicular = (cos_theta_i - eta * cos_theta_t) /
		(cos_theta_i + eta * cos_theta_t);
	float kr = 0.5 * (r_parallel * r_parallel +
		r_perpendicular * r_perpendicular);
	return kr;
}