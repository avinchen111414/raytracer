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
	virtual RGBColor global_shade(ShadeRec& sr);
	virtual void set_sampler(Sampler* sampler_ptr);

	void set_ka(const float ka);
	void set_kd(const float kd);
	void set_cd(const RGBColor& cd);
	void set_ks(const float ks);
	void set_cs(const RGBColor& cs);
	void set_exp(const float exp);

protected:
	Lambertian* ambient_brdf;
	Lambertian* diffuse_brdf;
	GlossySpecular* specular_brdf;
};

inline void Phong::set_ka(const float ka)
{
	if (ambient_brdf)
		ambient_brdf->SetKd(ka);
}

inline void Phong::set_kd(const float kd)
{
	if (diffuse_brdf)
		diffuse_brdf->SetKd(kd);
}

inline void Phong::set_cd(const RGBColor& cd)
{
	if (diffuse_brdf)
		diffuse_brdf->SetCd(cd);
	if (ambient_brdf)
		ambient_brdf->SetCd(cd);
}

inline void Phong::set_ks(const float ks)
{
	if (specular_brdf)
		specular_brdf->SetKs(ks);
}

inline void Phong::set_cs(const RGBColor& cs)
{
	if (specular_brdf)
		specular_brdf->SetCs(cs);
}

inline void Phong::set_exp(const float exp)
{
	if (specular_brdf)
		specular_brdf->SetExp(exp);
}

#endif