#include "arealighting.h"
#include "utilities/ray.h"
#include "utilities/utility.h"
#include "world/world.h"
#include "utilities/shaderec.h"
#include "materials/material.h"

AreaLighting::AreaLighting()
	: Tracer()
{

}
AreaLighting::AreaLighting(World* _world_ptr)
	: Tracer(_world_ptr)
{

}

AreaLighting::~AreaLighting()
{

}

RGBColor AreaLighting::trace_ray(const Ray& ray, const int depth) const
{
	if (depth > world_ptr->vp.GetMaxDepth())
		return world_ptr->background_color;

	ShadeRec sr(world_ptr->HitObject(ray));

	if (sr.hit_an_object)
	{
		sr.ray = ray;
		sr.depth = depth;
		return sr.material_ptr->area_light_shade(sr);
	}
	else
	{
		return world_ptr->background_color;
	}
}

RGBColor AreaLighting::trace_ray(const Ray& ray, float& tmin, const int depth) const
{
	if (depth > world_ptr->vp.GetMaxDepth())
		return world_ptr->background_color;

	ShadeRec sr(world_ptr->HitObject(ray));

	if (sr.hit_an_object)
	{
		sr.ray = ray;
		sr.depth = depth;
		tmin = sr.t;
		return sr.material_ptr->area_light_shade(sr);
	}
	else
	{
		tmin = FLT_MAX;
		return world_ptr->background_color;
	}
}

