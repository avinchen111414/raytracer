#include "arealighting.h"
#include "utilities/ray.h"
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
	if (depth > world_ptr->vp.max_depth)
		return 0.0f;

	ShadeRec sr(world_ptr->hit_object(ray));

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
