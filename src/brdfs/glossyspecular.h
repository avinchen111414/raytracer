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

	virtual GlossySpecular* Clone() const;

	virtual RGBColor F(const ShadeRec& sr, const Vector3D& wo, 
		const Vector3D& wi);

	virtual void SetSampler(Sampler* sampler_ptr);

	virtual RGBColor SampleF(const ShadeRec& sr, const Vector3D& wo, 
		Vector3D& wi, float& pdf);

	inline void SetKs(const float ks) {this->m_ks = ks;};
	inline void SetCs(const RGBColor& cs) {this->m_cs = cs;};
	inline void SetExp(const float exp) {this->m_exp = exp;};

private:
	float m_ks;
	RGBColor m_cs;
	float m_exp;
};

#endif