#ifndef __RAYCAST_H__
#define __RAYCAST_H__

#include "tracer.h"

class RayCast: public Tracer
{
public:
	RayCast();
	RayCast(World* world_ptr);
	virtual ~RayCast();

	virtual RGBColor trace_ray(const Ray& ray) const;
	virtual RGBColor trace_ray(const Ray& ray, const int depth) const;
};

#endif