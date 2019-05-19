#ifndef __FRESNELREFLECTOR_H__
#define __FRESNELREFLECTOR_H__

#include "BRDF.h"

class ShadeRec;
class Vector3D;
class FresnelReflector: public BRDF
{
public:
	FresnelReflector();

	FresnelReflector(const FresnelReflector& fr);

	FresnelReflector& operator= (const FresnelReflector& rhs);

	virtual ~FresnelReflector();

	virtual FresnelReflector* Clone() const;

	virtual RGBColor FresnelReflector::SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;

	float Fresnel(const ShadeRec& sr) const;

	inline void SetEta(float eta_in, float eta_out) {this->m_eta_in = eta_in; this->m_eta_out = eta_out;};
protected:

private:
	RGBColor m_cr;
	float m_eta_in;
	float m_eta_out;

};

#endif