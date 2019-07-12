#ifndef __SHADE_REC__
#define __SHADE_REC__

class Material;
class World;

#include "point3d.h"
#include "vector3d.h"
#include "normal.h"
#include "ray.h"
#include "rgbcolor.h"

class ShadeRec {
	public:
		bool				hit_an_object;		// Did the ray hit an object?

		Material* 			material_ptr;	// Pointer to the nearest object's material
		Point3D 			hit_point;			// World coordinates of intersection

		Point3D				local_hit_point;	// World coordinates of hit point on generic object (used for texture transformations)
		Normal				normal;				// Normal at hit point

		Ray					ray;				// Required for specular highlights and area lights
		int					depth;				// recursion depth
		Vector3D			dir;				// for area lights
		
		Vector3D			wi;					// Vector from hit point to sample point on area light
		Point3D				sample_point;		// Sample point on area light.
		Normal				light_normal;		// Normal of sample point on area light.

		float				t;					// ray parameter

		World&				w;					// World reference
		RGBColor            color;
				
		ShadeRec(World& wr);					// constructor
		ShadeRec(const ShadeRec& sr);			// copy constructor
		ShadeRec& operator= (const ShadeRec& rhs);
};

#endif