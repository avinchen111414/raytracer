#include "transparent.h"
#include "utilities/shaderec.h"
#include "world/world.h"
#include "lights/light.h"
#include "tracer/tracer.h"
#include "brdfs/perfectspecular.h"
#include "brdfs/perfecttransmitter.h"

Transparent::Transparent():
	Phong(), reflective_brdf(new PerfectSpecular()),
	transmitive_btdf(new PerfectTransmitter())
{

}

Transparent::Transparent(const Transparent& other):
	Phong(other), reflective_brdf(other.reflective_brdf),
	transmitive_btdf(other.transmitive_btdf)
{

}

Material* Transparent::clone() const
{
	return new Transparent(*this);
}

Transparent& Transparent::operator=(const Transparent& rhs)
{
	if (this == &rhs)
		return *this;

	Phong::operator=(rhs);

	reflective_brdf = rhs.reflective_brdf->clone();
	transmitive_btdf = rhs.transmitive_btdf->clone();
	return *this;
}

RGBColor Transparent::area_light_shade(ShadeRec& sr)
{
	RGBColor L = Phong::area_light_shade(sr);

	Vector3D wo = -sr.ray.d;
	Vector3D wi;
	RGBColor fr = reflective_brdf->sample_f(sr, wo, wi);

	Ray reflective_ray(sr.hit_point, wi);
	
	if (transmitive_btdf->tir(sr))
	{
		L += fr * sr.w.tracer_ptr->trace_ray(reflective_ray, sr.depth + 1) * 
			(sr.normal * wi);
	}
	else
	{
		L += fr * sr.w.tracer_ptr->trace_ray(reflective_ray, sr.depth + 1) * 
			(sr.normal * wi);
		
		Vector3D wt;
		RGBColor ft = transmitive_btdf->sample_f(sr, wo, wt);
		Ray transmitted_ray(sr.hit_point, wt);

		L += ft * sr.w.tracer_ptr->trace_ray(transmitted_ray, sr.depth + 1) *
			fabs(sr.normal * wt);
	}

	return L;
}