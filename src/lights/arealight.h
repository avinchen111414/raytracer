#ifndef __AREA_LIGHT_H__
#define __AREA_LIGHT_H__

#include "light.h"
#include "utilities/point3d.h"
#include "utilities/normal.h"
#include "geometricobjects/geometricobject.h"

class Material;

class AreaLight: public Light
{
public:
	AreaLight();
	AreaLight(const AreaLight& al);
	AreaLight& operator= (const AreaLight& rhs);
	~AreaLight();

	virtual Light* clone() const;
	virtual Vector3D get_direction(ShadeRec& sr);
	virtual RGBColor L(ShadeRec& sr);
	virtual bool in_shadow(const Ray& ray, const ShadeRec& sr);
	virtual float G(const ShadeRec& sr) const;
	virtual float pdf(const ShadeRec& sr) const;

	void set_object(GeometricObject* obj_ptr);

private:
	GeometricObject* object_ptr;
	Material* material_ptr;	// an emissive material

	Point3D sample_point;	// sample point on the surface
	Normal light_normal;	// normal of the sample point
	Vector3D wi;	//	unit vector from hit point to sample point

};

inline void AreaLight::set_object(GeometricObject* obj_ptr)
{
	object_ptr = obj_ptr;
	material_ptr = obj_ptr->get_material();	
}

#endif