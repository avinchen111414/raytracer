#ifndef __GLOSSY_SPECULAR_H__
#define __GLOSSY_SPECULAR_H__

#include "BRDF.h"
#include "utilities/vector3d.h"
#include "utilities/rgbcolor.h"

class ShadeRec;

class GlossySpecular: public BRDF
{
public:
	GlossySpecular();

	GlossySpecular(const GlossySpecular& glossy);

	GlossySpecular& operator= (const GlossySpecular& rhs);

	virtual ~GlossySpecular();

	virtual GlossySpecular* clone() const;

	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, 
		const Vector3D& wi);

	virtual void set_sampler(Sampler* sampler_ptr);

	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, 
		Vector3D& wi, float& pdf);

	inline void set_ks(const float ks) {this->ks = ks;};
	inline void set_cs(const RGBColor& cs) {this->cs = cs;};
	inline void set_exp(const float exp) {this->exp = exp;};

private:
	float ks;
	RGBColor cs;
	float exp;
};

#endif