#include "phong.h"
#include "utilities/shaderec.h"
#include "world/world.h"
#include "lights/light.h"
#include "tracer/tracer.h"

Phong::Phong():
	ambient_brdf(new Lambertian),
	diffuse_brdf(new Lambertian),
	specular_brdf(new GlossySpecular)
{

}

Phong::Phong(const Phong& p):
	Material(p)
{
	if (p.ambient_brdf)
		ambient_brdf = p.ambient_brdf->Clone();
	else
		ambient_brdf = nullptr;

	if (p.diffuse_brdf)
		diffuse_brdf = p.diffuse_brdf->Clone();
	else
		diffuse_brdf = nullptr;

	if (p.specular_brdf)
		specular_brdf = p.specular_brdf->Clone();
	else
		specular_brdf = nullptr;
}

Phong& Phong::operator=(const Phong& rhs)
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
		ambient_brdf = rhs.ambient_brdf->Clone();

	if (diffuse_brdf)
	{
		delete diffuse_brdf;
		diffuse_brdf = nullptr;
	}

	if (rhs.diffuse_brdf)
	{
		diffuse_brdf = rhs.diffuse_brdf->Clone();
	}

	if (specular_brdf)
	{
		delete specular_brdf;
		specular_brdf = nullptr;
	}

	if (rhs.specular_brdf)
	{
		specular_brdf = rhs.specular_brdf->Clone();
	}

	return *this;
}

RGBColor Phong::global_shade(ShadeRec& sr)
{
	RGBColor L;

	if (sr.depth == 0)
		L = area_light_shade(sr);

	Vector3D wo = -sr.ray.d;
	Vector3D wi;
	float pdf;
	RGBColor f = diffuse_brdf->SampleF(sr, wo, wi, pdf);
	float ndotwi = sr.normal * wi;
	Ray reflected_ray(sr.hit_point, wi);
	L += f * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1)
		* ndotwi / pdf;

	f = specular_brdf->SampleF(sr, wo, wi, pdf);
	ndotwi = sr.normal * wi;
	reflected_ray = Ray(sr.hit_point, wi);
	L += f * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1)
		* ndotwi / pdf;

	return L;
}

void Phong::set_sampler(Sampler* sampler_ptr)
{
	diffuse_brdf->SetSampler(sampler_ptr);
	specular_brdf->SetSampler(sampler_ptr);
}

RGBColor Phong::area_light_shade(ShadeRec& sr)
{
	Vector3D wo = -sr.ray.d;
	RGBColor L = ambient_brdf->Rho(sr, wo) * sr.w.ambient_ptr->L(sr);
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
				L += (diffuse_brdf->F(sr, wo, wi) +
					specular_brdf->F(sr, wo, wi)) * sr.w.lights[j]->L(sr) *
					ndotwi * sr.w.lights[j]->G(sr) / sr.w.lights[j]->pdf(sr);
			}
		}
	}

	return L;
}

RGBColor Phong::shade(ShadeRec& sr)
{
	Vector3D wo = -sr.ray.d;
	RGBColor L = ambient_brdf->Rho(sr, wo) * sr.w.ambient_ptr->L(sr);
	int num_lights = sr.w.lights.size();

	for (int j = 0; j != num_lights; j++)
	{
		Vector3D wi = sr.w.lights[j]->get_direction(sr);
		float n_dot_wi = static_cast<float>(sr.normal * wi);

		if (n_dot_wi > 0.0)
		{
			bool in_shadow = false;
			if (sr.w.lights[j]->cast_shadow() && this->recv_shadow)
			{
				Ray shadow_ray(sr.hit_point, wi);
				in_shadow = sr.w.lights[j]->in_shadow(shadow_ray, sr);
			}
	
			if (!in_shadow)
			{
				L += (diffuse_brdf->F(sr, wo, wi) + 
					specular_brdf->F(sr, wo, wi)) * sr.w.lights[j]->L(sr) * n_dot_wi;
			}
		}
	}

	return L;
}

Material* Phong::clone() const 
{
	return (new Phong(*this));
}