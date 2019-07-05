#include "arealight.h"
#include "geometricobjects/geometricobject.h"
#include "materials/material.h"
#include "utilities/shaderec.h"
#include "world/world.h"

AreaLight::AreaLight():
	Light(),
	object_ptr(nullptr),
	material_ptr(nullptr)
{

}

AreaLight::AreaLight(const AreaLight& al):
	Light(al)
{
	if (this->object_ptr)
		delete this->object_ptr;
	this->object_ptr = nullptr;
	if (al.object_ptr)
		this->object_ptr = al.object_ptr->Clone();
	
	if (this->material_ptr)
		delete this->material_ptr;
	this->material_ptr = nullptr;
	if (al.material_ptr)
		this->material_ptr = al.material_ptr->clone();
}

AreaLight& AreaLight::operator=(const AreaLight& rhs)
{
	if (this == &rhs)
		return (*this);

	Light::operator=(rhs);
	if (object_ptr)
	{
		delete object_ptr;
		object_ptr = nullptr;
	}
	if (rhs.object_ptr)
	{
		object_ptr = rhs.object_ptr->Clone();
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

AreaLight::~AreaLight()
{
	if (object_ptr)
		delete object_ptr;
	if (material_ptr)
		delete material_ptr;
}

Light* AreaLight::clone() const
{
	return new AreaLight(*this);
}

Vector3D AreaLight::get_direction(ShadeRec& sr)
{
	sample_point = object_ptr->Sample();
	light_normal = object_ptr->GetNormal(sample_point);
	wi = sample_point - sr.hit_point;
	wi.Normalize();
	return wi;
}

RGBColor AreaLight::L(ShadeRec& sr)
{
	// (NOTE) cause wi is the vector from hit point to light's sample point
	// so, light normal should be inverse. *
	float ndotd = static_cast<float>(-light_normal * wi);
	if (ndotd > 0.0f)
		return material_ptr->get_le(sr);
	else
		return 0.0f;
}

bool AreaLight::in_shadow(const Ray& ray, const ShadeRec& sr)
{
	float t;
	int num_objects = sr.w.objects.size();
	float ts = static_cast<float>((sample_point - ray.o) * ray.d);

	for (int j = 0; j != num_objects; j++)
	{
		if (sr.w.objects[j]->ShadowHit(ray, t) && t < ts)
			return true;
	}

	return false;
}

float AreaLight::G(const ShadeRec& sr) const
{
	// (NOTE) *
	float ndotd = static_cast<float>(-light_normal * wi);
	float d2 = static_cast<float>(sample_point.DidSquared(sr.hit_point));
	return ndotd / d2;
}

float AreaLight::pdf(const ShadeRec& sr) const
{
	return object_ptr->Pdf(sr);
}
