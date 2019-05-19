#include "utilities/shaderec.h"
#include "brdfs/fresnelreflector.h"
#include "brdfs/fresneltransmitter.h"
#include "utilities/utility.h"
#include "world/world.h"
#include "tracer/tracer.h"
#include "dielectric.h"

Dielectric::Dielectric():
	Phong(), fresnel_brdf(new FresnelReflector()),
	fresnel_btdf(new FresnelTransmitter())
{

}

Dielectric::Dielectric(const Dielectric& dlt):
	Phong(dlt)
{
	fresnel_brdf = dlt.fresnel_brdf->Clone();
	fresnel_btdf = dlt.fresnel_btdf->Clone();
}

Material* Dielectric::clone() const
{
	return new Dielectric(*this);
}

Dielectric& Dielectric::operator=(const Dielectric& rhs)
{
	if (this == &rhs)
		return *this;

	Phong::operator=(rhs);

	cf_in = rhs.cf_in; cf_out = rhs.cf_out;
	fresnel_brdf = rhs.fresnel_brdf->Clone();
	fresnel_btdf = rhs.fresnel_btdf->Clone();

	return *this;
}

RGBColor Dielectric::area_light_shade(ShadeRec& sr)
{
	//RGBColor L = Phong::area_light_shade(sr);
	RGBColor L(0.0f);

	Vector3D wi;
	Vector3D wo(-sr.ray.d);
	RGBColor fr = fresnel_brdf->SampleF(sr, wo, wi);
	Ray reflected_ray(sr.hit_point, wi);
	
	RGBColor Lr, Lt;
	float tmin = FLT_MAX;
	float ndotwi = static_cast<float>(sr.normal * wi);
	if (fresnel_btdf->Tir(sr))
	{
		// total internal reflection, kr always keeps to 1.0f
		if (ndotwi < 0.0)
		{
			// reflected ray is inside
			Lr = sr.w.tracer_ptr->trace_ray(reflected_ray, tmin, sr.depth + 1);
			L += cf_in.powc(tmin) * Lr;
		}
		else
		{
			// otherwise
			Lr = sr.w.tracer_ptr->trace_ray(reflected_ray, tmin, sr.depth + 1);
			L += cf_out.powc(tmin) * Lr;
		}
	}
	else
	{
		Vector3D wt;
		RGBColor ft = fresnel_btdf->SampleF(sr, wo, wt);
		Ray transmitted_ray(sr.hit_point, wt);
		float ndotwt = static_cast<float>(sr.normal * wt);

		if (ndotwi < 0.0)
		{
			// reflected ray is inside
			Lr = fr * sr.w.tracer_ptr->trace_ray(reflected_ray, tmin, sr.depth + 1) *
				fabs(ndotwi);
			L += cf_in.powc(tmin) * Lr;

			// transmitted ray is outside
			Lt = ft * sr.w.tracer_ptr->trace_ray(transmitted_ray, tmin, sr.depth + 1) *
				fabs(ndotwt);
			L += cf_out.powc(tmin) * Lt;

		}
		else
		{
			// otherwise
			Lr = fr * sr.w.tracer_ptr->trace_ray(reflected_ray, tmin, sr.depth + 1) *
				fabs(ndotwi);
			L += cf_out.powc(tmin) * Lr;

			// transmitted ray is outside
			Lt = ft * sr.w.tracer_ptr->trace_ray(transmitted_ray, tmin, sr.depth + 1) *
				fabs(ndotwt);
			L += cf_in.powc(tmin) * Lt;
		}
	}

	return L;
}

void Dielectric::set_eta(float eta_in, float eta_out)
{
	fresnel_brdf->SetEta(eta_in, eta_out);
	fresnel_btdf->SetEta(eta_in, eta_out);
}

void Dielectric::set_color_filter(RGBColor cf_in, RGBColor cf_out)
{
	this->cf_in = cf_in; this->cf_out = cf_out;
}