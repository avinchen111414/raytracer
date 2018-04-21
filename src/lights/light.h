#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "utilities/vector3d.h"
#include "utilities/rgbcolor.h"

class ShadeRec;
class Ray;

class Light
{
public:
	Light();
	Light(const Light& ls);
	Light& operator= (const Light& rhs);
	~Light();

	virtual Light* clone() const = 0;
	virtual Vector3D get_direction(ShadeRec& rc) = 0;
	virtual RGBColor L(ShadeRec& sr) = 0;
	inline bool cast_shadow() {return shadows;};
	inline void set_cast_shadow(bool enable) {shadows = enable;};
	virtual bool in_shadow(const Ray& ray, const ShadeRec& sr);
	
	// for area light
	virtual float G(const ShadeRec& sr) const;
	virtual float pdf(const ShadeRec& sr) const;
protected:
	bool shadows;
};

#endif