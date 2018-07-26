#ifndef __MATTE_H__
#define __MATTE_H__

#include "material.h"
#include "brdfs/lambertian.h"

class ShadeRec;

class Matte: public Material
{
public:
	Matte();
	Matte(const Matte& m);
	virtual Material* clone() const;
	virtual ~Matte();
	Matte& operator= (const Matte& rhs);

	virtual RGBColor shade(ShadeRec& sr);
	virtual RGBColor area_light_shade(ShadeRec& sr);
	virtual RGBColor global_shade(ShadeRec& sr);
	virtual void set_sampler(Sampler* sampler_ptr);

	void set_ka(const float ka);
	void set_kd(const float kd);
	void set_cd(const RGBColor& cd);
private:
	Lambertian* ambient_brdf;
	Lambertian* diffuse_brdf;
};

inline void Matte::set_ka(const float ka)
{
	if (ambient_brdf)
		ambient_brdf->set_kd(ka);
}

inline void Matte::set_kd(const float kd)
{
	if (diffuse_brdf)
		diffuse_brdf->set_kd(kd);
}

inline void Matte::set_cd(const RGBColor& cd)
{
	if (diffuse_brdf)
		diffuse_brdf->set_cd(cd);
	if (ambient_brdf)
		ambient_brdf->set_cd(cd);
}

#endif