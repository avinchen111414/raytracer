#ifndef __AREALIGHTING_H__
#define __AREALIGHTING_H__

#include "tracer.h"

class World;
class Ray;
class AreaLighting: public Tracer
{
public:
	AreaLighting();

	AreaLighting(World* _world_ptr);

	~AreaLighting();

	virtual RGBColor trace_ray(const Ray& ray, const int depth) const;
	virtual RGBColor trace_ray(const Ray& ray, float& tmin, const int depth) const;
};

#endif