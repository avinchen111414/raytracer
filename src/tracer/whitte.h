#ifndef __WHITTE_H__
#define __WHITTE_H__

#include "tracer.h"

class World;
class Ray;
class Whitte: public Tracer
{
public:
	Whitte();

	Whitte(World* _world_ptr);

	~Whitte();

	virtual RGBColor trace_ray(const Ray& ray, const int depth) const;
};

#endif