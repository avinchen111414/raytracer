#ifndef __PHONG_H__
#define __PHONG_H__

#include "material.h"
#include "brdfs/lambertian.h"
#include "brdfs/glossyspecular.h"

class ShadeRec;

class Phong: public Material
{
public:
	Phong();
	Phong(const Phong& p);
	virtual Material* clone() const;
	Phong& operator= (const Phong& rhs);

	virtual RGBColor shade(ShadeRec& sr);
	virtual RGBColor area_light_shade(ShadeRec& sr);

	void set_ka(const float ka);
	void set_kd(const float kd);
	void set_cd(const RGBColor& cd);
	void set_ks(const float ks);
	void set_cs(const RGBColor& cs);
	void set_exp(const float exp);

private:
	Lambertian* ambient_brdf;
	Lambertian* diffuse_brdf;
	GlossySpecular* specular_brdf;
};

inline void Phong::set_ka(const float ka)
{
	if (ambient_brdf)
		ambient_brdf->set_kd(ka);
}

inline void Phong::set_kd(const float kd)
{
	if (diffuse_brdf)
		diffuse_brdf->set_kd(kd);
}

inline void Phong::set_cd(const RGBColor& cd)
{
	if (diffuse_brdf)
		diffuse_brdf->set_cd(cd);
	if (ambient_brdf)
		ambient_brdf->set_cd(cd);
}

inline void Phong::set_ks(const float ks)
{
	if (specular_brdf)
		specular_brdf->set_ks(ks);
}

inline void Phong::set_cs(const RGBColor& cs)
{
	if (specular_brdf)
		specular_brdf->set_cs(cs);
}

inline void Phong::set_exp(const float exp)
{
	if (specular_brdf)
		specular_brdf->set_exp(exp);
}

#endif