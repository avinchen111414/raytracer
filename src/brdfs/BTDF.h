#ifndef __BTDF_H__
#define __BTDF_H__

#include "utilities/rgbcolor.h"

class ShadeRec;
class Vector3D;
class Sampler;

class BTDF
{
public:
	BTDF();
	BTDF(const BTDF& btdf_object);

	virtual RGBColor f(const ShadeRec& sr,
		const Vector3D& wo, const Vector3D& wi);

	virtual RGBColor sample_f(const ShadeRec& sr,
		const Vector3D& wo, Vector3D& wt);

	virtual RGBColor rho(const ShadeRec& sr,
		const Vector3D& wo);

	bool tir(const ShadeRec& sr) const;

protected:
	Sampler* sampler_ptr;
	float k;
	// Nin / Nout，Nout表示空气的ior（约等于1）
	float ior;
};

#endif