#include "ambientoccluder.h"
#include "sampler/sampler.h"
#include "sampler/jittered.h"
#include "utilities/shaderec.h"
#include "world/world.h"

AmbientOccluder::AmbientOccluder():
	Light(),
	sampler_ptr(nullptr),
	min_amount(0.2f, 0.2f, 0.2f),
	ls(1.0f),
	color(1.0f)
{
	
}

AmbientOccluder::AmbientOccluder(const AmbientOccluder& ao):
	Light(ao),
	min_amount(ao.min_amount),
	ls(ao.ls),
	color(ao.color)
{
	if (ao.sampler_ptr)
		sampler_ptr = ao.sampler_ptr->clone();
	else
		sampler_ptr = nullptr;
}

AmbientOccluder& AmbientOccluder::operator=(const AmbientOccluder& rhs)
{
	if (this == &rhs)
		return *this;

	Light::operator=(rhs);

	min_amount = rhs.min_amount;
	ls = rhs.ls;
	color = rhs.color;

	if (sampler_ptr)
		delete sampler_ptr;
	sampler_ptr = nullptr;
	if (rhs.sampler_ptr)
		sampler_ptr = rhs.sampler_ptr->clone();

	return *this;
}

Light* AmbientOccluder::clone() const
{
	return new AmbientOccluder(*this);
}

Vector3D AmbientOccluder::get_direction(ShadeRec& sr)
{
	Vector3D u, v, w;
	w = sr.normal;
	v = w ^ Vector3D(0.0072f, 1.0f, 0.0034f);
	v.Normalize();
	u = v ^ w;

	Point3D p = sampler_ptr->sample_hemisphere();
	return p.x * u + p.y * v + p.z * w;
}

void AmbientOccluder::set_sampler(Sampler* sp)
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

bool AmbientOccluder::in_shadow(const Ray& ray, const ShadeRec& sr)
{
	float t;
	int num_objects = sr.w.objects.size();

	for (size_t i = 0; i != num_objects; i++)
	{
		if (sr.w.objects[i]->ShadowHit(ray, t))
			return true;
	}

	return false;
}

RGBColor AmbientOccluder::L(ShadeRec& sr)
{
	Ray shadow_ray;
	shadow_ray.o = sr.hit_point;
	shadow_ray.d = get_direction(sr);

	if (in_shadow(shadow_ray, sr))
	{
		return min_amount * ls * color;
	}
	else
	{
		return ls * color;
	}
}