#include "singlesphere.h"
#include "world/world.h"
#include "utilities/ShadeRec.h"

SingleSphere::SingleSphere(void)
	: Tracer()
{}
		
SingleSphere::SingleSphere(World* _worldPtr)
	: Tracer(_worldPtr)
{}

SingleSphere::~SingleSphere(void) {}

RGBColor	
SingleSphere::trace_ray(const Ray& ray) const {
	ShadeRec	sr(*world_ptr); 	// not used
	double    	t;  				// not used
	
	if (world_ptr->sphere.hit(ray, t, sr))		
		return RGBColor(1.0, 0.0, 0.0);  
	else
		return RGBColor(0.0);   
}


