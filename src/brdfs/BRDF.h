#ifndef __BRDF_H__
#define __BRDF_H__

#include "utilities/rgbcolor.h"

class ShadeRec;
class Vector3D;
class Sampler;

class BRDF
{
public:
	BRDF();
	BRDF(const BRDF& brdf_object);
	~BRDF();
	BRDF& operator= (const BRDF& rhs);

	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, 
		const Vector3D& wi);

	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, 
		Vector3D& wi);

	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo);

	void set_sampler(Sampler* sampler_ptr);

protected:
	Sampler* sampler_ptr;
};

#endif