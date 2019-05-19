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
		Clone(void) const;

	virtual RGBColor
		F(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor												
		SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;

	virtual RGBColor
		SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

	virtual RGBColor
		Rho(const ShadeRec& sr, const Vector3D& wo) const;

	inline void SetKd(float kd) {this->m_kd = kd;};
	inline void SetCd(const RGBColor& cd) {this->m_cd = cd;};

private:
	float m_kd;
	RGBColor m_cd;
};

#endif