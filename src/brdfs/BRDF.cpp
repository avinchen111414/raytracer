#include "BRDF.h"
#include "utilities/shaderec.h"
#include "utilities/vector3d.h"
#include "sampler/sampler.h"

BRDF::BRDF()
	: m_sampler(nullptr)
{}

BRDF::BRDF(const BRDF& other)
{
	if (other.m_sampler)
		m_sampler = other.m_sampler->clone();
	else
		m_sampler = nullptr;
}

BRDF& BRDF::operator=(const BRDF& rhs) {
	if (this == &rhs)
		return (*this);

	if (m_sampler){
		delete m_sampler;
		m_sampler = nullptr;
	}

	if (rhs.m_sampler)
	{
		m_sampler = rhs.m_sampler->clone();
	}

	return (*this);
}

BRDF::~BRDF() 
{
	if (m_sampler)
	{
		delete m_sampler;
		m_sampler = nullptr;
	}
}

void BRDF::SetSampler(Sampler* sampler_ptr)
{
	if (sampler_ptr)
	{
		this->m_sampler = sampler_ptr;
		this->m_sampler->map_samples_to_hemisphere(1);
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