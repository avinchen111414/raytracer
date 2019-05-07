#include "reflective.h"
#include "utilities/shaderec.h"
#include "brdfs/perfectspecular.h"
#include "world/world.h"
#include "tracer/tracer.h"

Reflective::Reflective()
	: Phong(), reflective_brdf(new PerfectSpecular)
{

}

Reflective::Reflective(const Reflective& r)
	: Phong(r), reflective_brdf(new PerfectSpecular)
{

}

Material* Reflective::clone() const
{
	return new Reflective(*this);
}

Reflective& Reflective::operator= (const Reflective& rhs)
{
	if (this == &rhs)
		return *this;

	Phong::operator=(rhs);
	reflective_brdf = rhs.reflective_brdf->clone();
	return *this;
}

RGBColor Reflective::global_shade(ShadeRec& sr)
{
	RGBColor L(Phong::area_light_shade(sr));

	Vector3D wo = -sr.ray.d;
	Vector3D wi;
	float pdf;
	RGBColor fr = reflective_brdf->SampleF(sr, wo, wi, pdf);
	Ray reflected_ray(sr.hit_point, wi);

	if (sr.depth == 0)
		return L + fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 2)
			* (sr.normal * wi) / pdf;
	else
		return L + fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1)
			* (sr.normal * wi) / pdf;
}

RGBColor Reflective::area_light_shade(ShadeRec& sr)
{
	RGBColor L(Phong::area_light_shade(sr));

	Vector3D wo = -sr.ray.d;
	Vector3D wi;
	RGBColor fr = reflective_brdf->SampleF(sr, wo, wi);

	Ray reflective_ray(sr.hit_point, wi);
	
	L += fr * sr.w.tracer_ptr->trace_ray(reflective_ray, sr.depth + 1) * (sr.normal * wi);
	return L;
}

void Reflective::set_kr(const float kr)
{
	reflective_brdf->set_kr(kr);
	specular_brdf->set_ks(kr);
}

void Reflective::set_cr(const RGBColor& cr)
{
	reflective_brdf->set_cr(cr);
	specular_brdf->set_cs(cr);
}