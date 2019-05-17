#include "glossyspecular.h"
#include "utilities/shaderec.h"
#include "sampler/sampler.h"
#include <cmath>

using namespace std;

GlossySpecular::GlossySpecular():
	BRDF(),
	ks(1.0),
	cs(1.0),
	exp(1.0)
{
	
}

GlossySpecular::GlossySpecular(const GlossySpecular& glossy):
	BRDF(glossy),
	ks(glossy.ks),
	cs(glossy.cs),
	exp(glossy.exp)
{
	
}

GlossySpecular& GlossySpecular::operator=(const GlossySpecular& rhs)
{
	if (this == &rhs)
		return *this;

	BRDF::operator=(rhs);

	ks = rhs.ks;
	cs = rhs.cs;
	exp = rhs.exp;

	return *this;
}

GlossySpecular::~GlossySpecular()
{

}

GlossySpecular* GlossySpecular::clone() const
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
		L = ks * cs * pow(r_dot_wo, exp);

	return L;
}

void GlossySpecular::SetSampler(Sampler* sampler_ptr)
{
	if (sampler_ptr)
	{
		this->m_sampler_ptr = sampler_ptr;
		this->m_sampler_ptr->map_samples_to_hemisphere(exp);
	}	
}

RGBColor GlossySpecular::SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi,
								  float& pdf)
{
	float ndotwo = sr.normal * wo;
	Vector3D r = -wo + 2.0 * sr.normal * ndotwo;

	Vector3D w = r;
	w.normalize();
	Vector3D u = Vector3D(0.00424, 1, 0.00764) ^ w; 
	u.normalize();
	Vector3D v = u ^ w;

	Point3D sp = m_sampler_ptr->sample_hemisphere();
	wi = sp.x * u + sp.y * v + sp.z * w;

	if (sr.normal * wi < 0.0) 						// reflected ray is below tangent plane
		wi = -sp.x * u - sp.y * v + sp.z * w;

	float phong_lobe = pow(r * wi, exp);
	pdf = phong_lobe * (sr.normal * wi);

	return (ks * cs * phong_lobe);
}