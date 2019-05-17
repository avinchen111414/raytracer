#include "BRDF.h"
#include "utilities/shaderec.h"
#include "utilities/vector3d.h"
#include "sampler/sampler.h"

BRDF::BRDF()
	: m_sampler_ptr(nullptr)
{}

BRDF::BRDF(const BRDF& object)
{
	if (object.m_sampler_ptr)
		m_sampler_ptr = object.m_sampler_ptr->clone();
	else
		m_sampler_ptr = nullptr;
}

BRDF& BRDF::operator=(const BRDF& rhs) {
	if (this == &rhs)
		return (*this);

	if (m_sampler_ptr){
		delete m_sampler_ptr;
		m_sampler_ptr = nullptr;
	}

	if (rhs.m_sampler_ptr)
	{
		m_sampler_ptr = rhs.m_sampler_ptr->clone();
	}

	return (*this);
}

BRDF::~BRDF() 
{
	if (m_sampler_ptr)
	{
		delete m_sampler_ptr;
		m_sampler_ptr = nullptr;
	}
}

void BRDF::SetSampler(Sampler* sampler_ptr)
{
	if (sampler_ptr)
	{
		this->m_sampler_ptr = sampler_ptr;
		this->m_sampler_ptr->map_samples_to_hemisphere(1);
	}
}

RGBColor BRDF::F(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi)
{
	return RGBColor(0);
}

RGBColor BRDF::SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi)
{
	return RGBColor(0);
}

RGBColor BRDF::SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf)
{
	return RGBColor(0);
}

RGBColor BRDF::Rho(const ShadeRec& sr, const Vector3D& wo)
{
	return RGBColor(0);
}