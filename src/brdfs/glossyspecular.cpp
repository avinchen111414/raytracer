#include "glossyspecular.h"
#include "utilities/shaderec.h"
#include "sampler/sampler.h"
#include <cmath>

using namespace std;

GlossySpecular::GlossySpecular():
	BRDF(),
	m_ks(1.0),
	m_cs(1.0),
	m_exp(1.0)
{
	
}

GlossySpecular::GlossySpecular(const GlossySpecular& glossy):
	BRDF(glossy),
	m_ks(glossy.m_ks),
	m_cs(glossy.m_cs),
	m_exp(glossy.m_exp)
{
	
}

GlossySpecular& GlossySpecular::operator=(const GlossySpecular& rhs)
{
	if (this == &rhs)
		return *this;

	BRDF::operator=(rhs);

	m_ks = rhs.m_ks;
	m_cs = rhs.m_cs;
	m_exp = rhs.m_exp;

	return *this;
}

GlossySpecular::~GlossySpecular()
{

}

GlossySpecular* GlossySpecular::Clone() const
{
	return (new GlossySpecular(*this));
}

RGBColor GlossySpecular::F(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi)
{
	RGBColor L(0.0);
	float n_dot_wi = static_cast<float>(sr.normal * wi);
	Vector3D r = -wi + 2.0 * (sr.normal * n_dot_wi);
	float r_dot_wo = static_cast<float>(r * wo);

	if (r_dot_wo > 0.0)
		L = m_ks * m_cs * pow(r_dot_wo, m_exp);

	return L;
}

void GlossySpecular::SetSampler(Sampler* sampler_ptr)
{
	if (sampler_ptr)
	{
		this->m_sampler_ptr = sampler_ptr;
		this->m_sampler_ptr->MapSamplesToHemisphere(m_exp);
	}	
}

RGBColor GlossySpecular::SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi,
								  float& pdf)
{
	float ndotwo = sr.normal * wo;
	Vector3D r = -wo + 2.0 * sr.normal * ndotwo;

	Vector3D w = r;
	w.Normalize();
	Vector3D u = Vector3D(0.00424, 1, 0.00764) ^ w; 
	u.Normalize();
	Vector3D v = u ^ w;

	Point3D sp = m_sampler_ptr->SampleHemisphere();
	wi = sp.x * u + sp.y * v + sp.z * w;

	if (sr.normal * wi < 0.0) 						// reflected ray is below tangent plane
		wi = -sp.x * u - sp.y * v + sp.z * w;

	float phong_lobe = pow(r * wi, m_exp);
	pdf = phong_lobe * (sr.normal * wi);

	return (m_ks * m_cs * phong_lobe);
}