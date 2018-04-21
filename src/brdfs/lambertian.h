#ifndef __LAMBERTIAN_H__
#define __LAMBERTIAN_H__

#include "utilities/rgbcolor.h"
#include "BRDF.h"

class Lambertian: public BRDF
{
public:
	Lambertian(void);

	Lambertian(const Lambertian& lamb);

	Lambertian& 
		operator= (const Lambertian& rhs);

	virtual
		~Lambertian(void);

	virtual Lambertian*
		clone(void) const;

	virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor												
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;

	virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo) const;

	inline void set_kd(float kd) {this->kd = kd;};
	inline void set_cd(const RGBColor& cd) {this->cd = cd;};

private:
	float kd;
	RGBColor cd;
};

#endif