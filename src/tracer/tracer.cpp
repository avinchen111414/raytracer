#include "tracer.h"
#include "world/world.h"
#include "utilities/ray.h"

#include "Tracer.h"

// -------------------------------------------------------------------- default constructor

Tracer::Tracer(void)
	: world_ptr(nullptr)
{}


// -------------------------------------------------------------------- constructor

Tracer::Tracer(World* _worldPtr)
	: world_ptr(_worldPtr)
{}



// -------------------------------------------------------------------- destructor

Tracer::~Tracer(void) {
	if (world_ptr)
		world_ptr = nullptr;
}


// -------------------------------------------------------------------- trace_ray

RGBColor	
Tracer::trace_ray(const Ray& ray) const {
	return 0.0;
}


// -------------------------------------------------------------------- trace_ray

RGBColor	
Tracer::trace_ray(const Ray& ray, const int depth) const {
	return 0.0;
}

RGBColor
Tracer::trace_ray(const Ray& ray, float& tmin, const int depth) const {
	return 0.0;
}


