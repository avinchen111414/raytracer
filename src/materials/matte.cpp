#include "matte.h"
#include "utilities/shaderec.h"
#include "brdfs/lambertian.h"
#include "world/world.h"
#include "lights/light.h"
#include "tracer/tracer.h"

Matte::Matte():
	ambient_brdf(new Lambertian),
	diffuse_brdf(new Lambertian)
{

}

Matte::Matte(const Matte& m):
	Material(m)
{
	if (m.ambient_brdf)
		ambient_brdf = m.ambient_brdf->clone();
	else
		ambient_brdf = nullptr;

	if (m.diffuse_brdf)
		diffuse_brdf = m.diffuse_brdf->clone();
	else
		diffuse_brdf = nullptr;
}

Material* Matte::clone() const
{
	return new Matte(*this);
}

Matte& Matte::operator=(const Matte& rhs) 
{
	if (this == &rhs)
		return *this;

	Material::operator=(rhs);

	if (ambient_brdf) 
	{
		delete ambient_brdf;
		ambient_brdf = nullptr;
	}

	if (rhs.ambient_brdf)
		ambient_brdf = rhs.ambient_brdf->clone();

	if (diffuse_brdf)
	{
		delete diffuse_brdf;
		diffuse_brdf = nullptr;
	}

	if (rhs.diffuse_brdf)
	{
		diffuse_brdf = rhs.diffuse_brdf->clone();
	}

	return *this;
}

Matte::~Matte()
{
	if (ambient_brdf)
	{
		delete ambient_brdf;
		ambient_brdf = nullptr;
	}
	if (diffuse_brdf)
	{
		delete diffuse_brdf;
		diffuse_brdf = nullptr;
	}
};

RGBColor Matte::shade(ShadeRec& sr)
{
	Vector3D wo = -sr.ray.d;
	RGBColor L = ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
	int num_lights = sr.w.lights.size();

	for (int j = 0; j != num_lights; j++)
	{
		Vector3D wi = sr.w.lights[j]->get_direction(sr);
		float ndotwi = static_cast<float>(sr.normal * wi);

		if (ndotwi > 0.0)
		{
			bool in_shadow = false;
			if (sr.w.lights[j]->cast_shadow() && this->recv_shadow)
			{
				Ray shadow_ray(sr.hit_point, wi);
				in_shadow = sr.w.lights[j]->in_shadow(shadow_ray, sr);
			}

			if (!in_shadow)
			{
				L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[j]->L(sr) * ndotwi;
			}
		}
	}

	return L;
}

RGBColor Matte::area_light_shade(ShadeRec& sr)
{
	Vector3D wo = -sr.ray.d;
	RGBColor L = ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
	int num_lights = sr.w.lights.size();

	for (int j = 0; j != num_lights; j++)
	{
		Vector3D wi = sr.w.lights[j]->get_direction(sr);
		float ndotwi = static_cast<float>(sr.normal * wi);

		if (ndotwi > 0.0)
		{
			bool in_shadow = false;
			if (sr.w.lights[j]->cast_shadow() && this->recv_shadow)
			{
				Ray shadow_ray(sr.hit_point, wi);
				in_shadow = sr.w.lights[j]->in_shadow(shadow_ray, sr);
			}

			if (!in_shadow)
			{
				L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[j]->L(sr) *
					ndotwi * sr.w.lights[j]->G(sr) / sr.w.lights[j]->pdf(sr);
			}
		}
	}

	return L;
}

void Matte::set_sampler(Sampler* sampler_ptr)
{
	diffuse_brdf->set_sampler(sampler_ptr);
}

RGBColor Matte::global_shade(ShadeRec& sr)
{
	RGBColor L;

	if (sr.depth == 0)
		L = area_light_shade(sr);

	Vector3D wo = -sr.ray.d;
	Vector3D wi;
	float pdf;
	RGBColor f = diffuse_brdf->sample_f(sr, wo, wi, pdf);
	float ndotwi = sr.normal * wi;
	Ray reflected_ray(sr.hit_point, wi);
	L += f * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1)
		* ndotwi / pdf;

	return L;
}