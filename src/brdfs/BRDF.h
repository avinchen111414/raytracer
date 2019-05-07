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
	BRDF(const BRDF& other);
	~BRDF();
	BRDF& operator= (const BRDF& rhs);

	virtual RGBColor F(const ShadeRec& sr, const Vector3D& wo, 
		const Vector3D& wi);

	virtual RGBColor SampleF(const ShadeRec& sr, const Vector3D& wo, 
		Vector3D& wi);

	virtual RGBColor SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi,
		float& pdf);

	virtual RGBColor Rho(const ShadeRec& sr, const Vector3D& wo);

	virtual void SetSampler(Sampler* sampler_ptr);

	virtual BRDF* Clone() const;

protected:
	Sampler* m_sampler;
};

#endif