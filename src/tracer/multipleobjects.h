#ifndef __MULTIPLEOBJECTS_H__
#define __MULTIPLEOBJECTS_H__

#include "tracer.h"

class MultipleObjects: public Tracer
{
public:
	MultipleObjects();
	MultipleObjects(World* _world_ptr);

	virtual RGBColor trace_ray(const Ray& ray) const;
};

#endif