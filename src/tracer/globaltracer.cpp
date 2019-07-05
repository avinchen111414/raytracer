#include "globaltracer.h"
#include "utilities/ray.h"
#include "world/world.h"
#include "utilities/shaderec.h"
#include "materials/material.h"

GlobalTracer::GlobalTracer()
	: Tracer()
{

}

GlobalTracer::GlobalTracer(World* _world_ptr)
	: Tracer(_world_ptr)
{

}

GlobalTracer::~GlobalTracer()
{

}

RGBColor GlobalTracer::trace_ray(const Ray& ray, const int depth) const
{
	if (depth > world_ptr->vp.m_max_depth)
		return world_ptr->background_color;

	ShadeRec sr(world_ptr->HitObject(ray));

	if (sr.hit_an_object)
	{
		sr.ray = ray;
		sr.depth = depth;
		return sr.material_ptr->global_shade(sr);
	}
	else
	{
		return world_ptr->background_color;
	}
}