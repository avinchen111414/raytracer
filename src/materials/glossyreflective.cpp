#include "glossyreflective.h"
#include "utilities/shaderec.h"
#include "world/world.h"
#include "tracer/tracer.h"
#include "sampler/sampler.h"

GlossyReflective::GlossyReflective()
	: Phong(), glossy_reflective_brdf(new GlossySpecular)
{

}

GlossyReflective::GlossyReflective(const GlossyReflective& r)
	: Phong(r), glossy_reflective_brdf(new GlossySpecular)
{

}

Material* GlossyReflective::clone() const
{
	return new GlossyReflective(*this);
}

GlossyReflective& GlossyReflective::operator= (const GlossyReflective& rhs)
{
	if (this == &rhs)
		return *this;

	Material::operator=(rhs);
	glossy_reflective_brdf = rhs.glossy_reflective_brdf->Clone();
	return *this;
}

RGBColor GlossyReflective::global_shade(ShadeRec& sr)
{
	RGBColor L(Phong::area_light_shade(sr));

	Vector3D wo = -sr.ray.d;
	Vector3D wi;
	float pdf;
	RGBColor fr = glossy_reflective_brdf->SampleF(sr, wo, wi, pdf);
	Ray reflected_ray(sr.hit_point, wi);

	if (sr.depth == 0)
		return L + sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 2)
			* (sr.normal * wi) / pdf;
	else
		return L + sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1)
			* (sr.normal * wi) / pdf;
}

RGBColor GlossyReflective::area_light_shade(ShadeRec& sr)
{
	RGBColor L(Phong::area_light_shade(sr));
	Vector3D wo(-sr.ray.d);
	Vector3D wi;
	float pdf;
	RGBColor fr = glossy_reflective_brdf->SampleF(sr, wo, wi, pdf);

	Ray reflective_ray(sr.hit_point, wi);

	// sr.w.tracer_ptr->trace_ray stands for the Lo(rc(p, wi,j), -wi,j)
	RGBColor Lo = sr.w.tracer_ptr->trace_ray(reflective_ray, sr.depth + 1);
	L += fr * Lo *
		(sr.normal * wi) / pdf;

	return L;
}
void GlossyReflective::set_kr(const float kr)
{
	Phong::set_ks(kr);
	glossy_reflective_brdf->SetKs(kr);
}

void GlossyReflective::set_cr(const float cr)
{
	Phong::set_cd(cr);
	glossy_reflective_brdf->SetCs(cr);
}

void GlossyReflective::set_exp(const float exp)
{
	Phong::set_exp(exp);
	glossy_reflective_brdf->SetExp(exp);
}

void GlossyReflective::set_sampler(Sampler* sampler)
{
	glossy_reflective_brdf->SetSampler(sampler);
}