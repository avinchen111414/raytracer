#ifndef __GLOBALTRACER_H__
#define __GLOBALTRACER_H__

#include "tracer.h"

class World;
class Ray;
class GlobalTracer: public Tracer
{
public:
	GlobalTracer();

	GlobalTracer(World* _world_ptr);

	~GlobalTracer();

	virtual RGBColor trace_ray(const Ray& ray, const int depth) const;

};

#endif