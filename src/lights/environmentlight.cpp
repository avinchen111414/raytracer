#include "environmentlight.h"
#include "sampler/sampler.h"
#include "materials/material.h"
#include "utilities/shaderec.h"
#include "world/world.h"

EnvironmentLight::EnvironmentLight()
	: Light(), sampler_ptr(NULL), material_ptr(NULL)
{
}

EnvironmentLight::EnvironmentLight(const EnvironmentLight& env_light)
	: Light(env_light)
{
	if (env_light.sampler_ptr)
		sampler_ptr = env_light.sampler_ptr->clone();
	if (env_light.material_ptr)
		material_ptr = env_light.material_ptr->clone();
}

EnvironmentLight& EnvironmentLight::operator= (const EnvironmentLight& rhs)
{
	if (this == &rhs)
		return *this;

	Light::operator=(rhs);
	if (sampler_ptr)
	{
		delete sampler_ptr;
		sampler_ptr = nullptr;
	}
	if (rhs.sampler_ptr)
	{
		sampler_ptr = rhs.sampler_ptr->clone();
	}

	if (material_ptr)
	{
		delete material_ptr;
		material_ptr = nullptr;
	}
	if (rhs.material_ptr)
	{
		material_ptr = rhs.material_ptr->clone();
	}
	return *this;
}

EnvironmentLight::~EnvironmentLight()
{
	if (sampler_ptr)
		delete sampler_ptr;
	if (material_ptr)
		delete material_ptr;
}

Light* EnvironmentLight::clone() const
{
	return new EnvironmentLight(*this);
}

void EnvironmentLight::set_sampler(Sampler* sp)
{
	if (sampler_ptr)
	{
		delete sampler_ptr;
		sampler_ptr = nullptr;
	}

	if (sp)
	{
		sampler_ptr = sp;
		sampler_ptr->map_samples_to_hemisphere(1);
	}
}

Vector3D EnvironmentLight::get_direction(ShadeRec& rc)
{
	Vector3D u, v, w;
	w = rc.normal;
	v = w ^ Vector3D(0.0072f, 1.0f, 0.0034f);
	v.normalize();
	u = v ^ w;
	
	Point3D p = sampler_ptr->sample_hemisphere();
	return p.x * u + p.y * v + p.z * w;
}

RGBColor EnvironmentLight::L(ShadeRec& sr)
{
	return material_ptr->get_le(sr);
}

bool EnvironmentLight::in_shadow(const Ray& ray, const ShadeRec& sr)
{
	float t;
	int num_objects = sr.w.objects.size();

	for (size_t i = 0; i != num_objects; i++)
	{
		if (sr.w.objects[i]->shadow_hit(ray, t))
			return true;
	}

	return false;
}