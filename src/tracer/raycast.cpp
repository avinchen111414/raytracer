#include "raycast.h"
#include "utilities/shaderec.h"
#include "world/world.h"
#include "materials/material.h"

RayCast::RayCast():
	Tracer()
{

}

RayCast::RayCast(World* world_ptr):
	Tracer(world_ptr)
{

}

RayCast::~RayCast()
{

}

RGBColor RayCast::trace_ray(const Ray& ray) const
{
	return trace_ray(ray, 0);
}

RGBColor RayCast::trace_ray(const Ray& ray, const int depth) const
{
	ShadeRec sr(world_ptr->HitObject(ray));
	if (sr.hit_an_object) {
		sr.ray = ray;
		return (sr.material_ptr->shade(sr));
	}
	else
	{
		return world_ptr->background_color;
	}
}