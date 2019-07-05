#include "multipleobjects.h"
#include "utilities/ray.h"
#include "utilities/shaderec.h"
#include "world/world.h"

MultipleObjects::MultipleObjects()
	: Tracer()
{}

MultipleObjects::MultipleObjects(World* world_ptr)
	: Tracer(world_ptr)
{}

RGBColor MultipleObjects::trace_ray(const Ray& ray) const
{
	ShadeRec sr(world_ptr->HitBareBonesObjects(ray));

	if (sr.hit_an_object)
		return sr.color;
	else
		return world_ptr->background_color;
}