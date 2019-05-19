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

	reflective_brdf = rhs.reflective_brdf->Clone();
	transmitive_btdf = rhs.transmitive_btdf->Clone();
	return *this;
}

RGBColor Transparent::area_light_shade(ShadeRec& sr)
{
	//RGBColor L = Phong::area_light_shade(sr);
	RGBColor L(0.0f);

	Vector3D wo = -sr.ray.d;
	Vector3D wi;
	RGBColor fr = reflective_brdf->SampleF(sr, wo, wi);

	Ray reflective_ray(sr.hit_point, wi);
	
	if (transmitive_btdf->Tir(sr))
	{
		L += sr.w.tracer_ptr->trace_ray(reflective_ray, sr.depth + 1);
	}
	else
	{	
		Vector3D wt;
		RGBColor ft = transmitive_btdf->SampleF(sr, wo, wt);
		Ray transmitted_ray(sr.hit_point, wt);
		
		L += fr * sr.w.tracer_ptr->trace_ray(reflective_ray, sr.depth + 1) * 
			fabs(sr.normal * wi);
		L += ft * sr.w.tracer_ptr->trace_ray(transmitted_ray, sr.depth + 1) *
			fabs(sr.normal * wt);

		if (L.r > L.g && L.r > L.b && L.r >= 0.5 && L.g <= 0.2 && L.b <= 0.2)
			bool _break = true;
	}

	return L;
}

void Transparent::set_kr(float kr)
{
	reflective_brdf->SetKr(kr);
}

void Transparent::set_cr(const RGBColor& cr)
{
	reflective_brdf->SetCr(cr);
}

void Transparent::set_kt(float kt)
{
	transmitive_btdf->SetKt(kt);
}

void Transparent::set_ior(float ior)
{
	transmitive_btdf->SetIor(ior);
}