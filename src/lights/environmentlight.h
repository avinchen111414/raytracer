#ifndef __ENVIRONMENTLIGHT_H__
#define __ENVIRONMENTLIGHT_H__

#include "light.h"
#include "utilities/point3d.h"

class Sampler;
class Material;

class EnvironmentLight: public Light
{
public:
	EnvironmentLight();
	EnvironmentLight(const EnvironmentLight& env_light);
	EnvironmentLight& operator= (const EnvironmentLight& rhs);
	~EnvironmentLight();

	virtual Light* clone() const;
	virtual Vector3D get_direction(ShadeRec& rc);
	virtual RGBColor L(ShadeRec& sr);
	virtual bool in_shadow(const Ray& ray, const ShadeRec& sr);

	void set_sampler(Sampler* sp);
	void set_material(Material* mtl) { material_ptr = mtl; }

private:
	Sampler *sampler_ptr;
	Material *material_ptr;
};


#endif