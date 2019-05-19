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

	virtual RGBColor F(const ShadeRec& sr,
		const Vector3D& wo, const Vector3D& wi);

	virtual RGBColor SampleF(const ShadeRec& sr,
		const Vector3D& wo, Vector3D& wt);

	virtual RGBColor Rho(const ShadeRec& sr,
		const Vector3D& wo);

	virtual bool Tir(const ShadeRec& sr) const;

	void SetKt(float kt) { m_k = kt; };
	void SetIor(float _ior) { m_ior = _ior; };

protected:
	Sampler* m_sampler_ptr;
	float m_k;
	// Nin / Nout，Nout表示空气的ior（约等于1）
	float m_ior;
};

#endif