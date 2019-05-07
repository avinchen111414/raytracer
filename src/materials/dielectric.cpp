#include "utilities/shaderec.h"
#include "brdfs/fresnelreflector.h"
#include "brdfs/fresneltransmitter.h"
#include "utilities/utility.h"
#include "world/world.h"
#include "tracer/tracer.h"
#include "dielectric.h"

Dielectric::Dielectric():
	Phong(), m_fresnel_brdf(new FresnelReflector()),
	m_fresnel_btdf(new FresnelTransmitter())
{

}

Dielectric::Dielectric(const Dielectric& dlt):
	Phong(dlt)
{
	m_fresnel_brdf = dynamic_cast<FresnelReflector*>(dlt.m_fresnel_brdf->Clone());
	m_fresnel_btdf = dlt.m_fresnel_btdf->clone();
}

Material* Dielectric::Clone() const
{
	return new Dielectric(*this);
}

Dielectric& Dielectric::operator=(const Dielectric& rhs)
{
	if (this == &rhs)
		return *this;

	Phong::operator=(rhs);

	m_cf_in = rhs.m_cf_in; m_cf_out = rhs.m_cf_out;
	m_fresnel_brdf = dynamic_cast<FresnelReflector*>(rhs.m_fresnel_brdf->Clone());
	m_fresnel_btdf = rhs.m_fresnel_btdf->clone();

	return *this;
}

RGBColor Dielectric::AreaLightShade(ShadeRec& sr)
{
	//RGBColor L = Phong::area_light_shade(sr);
	RGBColor L(0.0f);

	Vector3D wi;
	Vector3D wo(-sr.ray.d);
	RGBColor fr = m_fresnel_brdf->SampleF(sr, wo, wi);
	Ray reflected_ray(sr.hit_point, wi);
	
	RGBColor Lr, Lt;
	float tmin = FLT_MAX;
	float ndotwi = static_cast<float>(sr.normal * wi);
	if (m_fresnel_btdf->Tir(sr))
	{
		// total internal reflection, kr always keeps to 1.0f
		if (ndotwi < 0.0)
		{
			// reflected ray is inside
			Lr = sr.w.tracer_ptr->trace_ray(reflected_ray, tmin, sr.depth + 1);
			L += m_cf_in.powc(tmin) * Lr;
		}
		else
		{
			// otherwise
			Lr = sr.w.tracer_ptr->trace_ray(reflected_ray, tmin, sr.depth + 1);
			L += m_cf_out.powc(tmin) * Lr;
		}
	}
	else
	{
		Vector3D wt;
		RGBColor ft = m_fresnel_btdf->SampleF(sr, wo, wt);
		Ray transmitted_ray(sr.hit_point, wt);
		float ndotwt = static_cast<float>(sr.normal * wt);

		if (ndotwi < 0.0)
		{
			// reflected ray is inside
			Lr = fr * sr.w.tracer_ptr->trace_ray(reflected_ray, tmin, sr.depth + 1) *
				fabs(ndotwi);
			L += m_cf_in.powc(tmin) * Lr;

			// transmitted ray is outside
			Lt = ft * sr.w.tracer_ptr->trace_ray(transmitted_ray, tmin, sr.depth + 1) *
				fabs(ndotwt);
			L += m_cf_out.powc(tmin) * Lt;

		}
		else
		{
			// otherwise
			Lr = fr * sr.w.tracer_ptr->trace_ray(reflected_ray, tmin, sr.depth + 1) *
				fabs(ndotwi);
			L += m_cf_out.powc(tmin) * Lr;

			// transmitted ray is outside
			Lt = ft * sr.w.tracer_ptr->trace_ray(transmitted_ray, tmin, sr.depth + 1) *
				fabs(ndotwt);
			L += m_cf_in.powc(tmin) * Lt;
		}
	}

	return L;
}

void Dielectric::SetEta(float eta_in, float eta_out)
{
	m_fresnel_brdf->set_eta(eta_in, eta_out);
	m_fresnel_btdf->set_eta(eta_in, eta_out);
}

void Dielectric::SetColorFilter(RGBColor cf_in, RGBColor cf_out)
{
	this->m_cf_in = cf_in; this->m_cf_out = cf_out;
}