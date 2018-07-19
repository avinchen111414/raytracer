#include "BRDF.h"
#include "utilities/shaderec.h"
#include "utilities/vector3d.h"
#include "sampler/sampler.h"

BRDF::BRDF()
	: sampler_ptr(nullptr)
{}

BRDF::BRDF(const BRDF& object)
{
	if (object.sampler_ptr)
		sampler_ptr = object.sampler_ptr->clone();
	else
		sampler_ptr = nullptr;
}

BRDF& BRDF::operator=(const BRDF& rhs) {
	if (this == &rhs)
		return (*this);

	if (sampler_ptr){
		delete sampler_ptr;
		sampler_ptr = nullptr;
	}

	if (rhs.sampler_ptr)
	{
		sampler_ptr = rhs.sampler_ptr->clone();
	}

	return (*this);
}

BRDF::~BRDF() 
{
	if (sampler_ptr)
	{
		delete sampler_ptr;
		sampler_ptr = nullptr;
	}
}

void BRDF::set_sampler(Sampler* sampler_ptr)
{
	if (sampler_ptr)
	{
		this->sampler_ptr = sampler_ptr;
		this->sampler_ptr->map_samples_to_hemisphere(1);
	}
}

RGBColor BRDF::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi)
{
	return RGBColor(0);
}

RGBColor BRDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi)
{
	return RGBColor(0);
}

RGBColor BRDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf)
{
	return RGBColor(0);
}

RGBColor BRDF::rho(const ShadeRec& sr, const Vector3D& wo)
{
	return RGBColor(0);
}