#ifndef __PERFECTSPECULAR_H__
#define __PERFECTSPECULAR_H__

#include "BRDF.h"

class PerfectSpecular: public BRDF
{
public:
	PerfectSpecular();

	PerfectSpecular(const PerfectSpecular& pr);

	PerfectSpecular& operator= (const PerfectSpecular& rhs);

	virtual ~PerfectSpecular();

	virtual PerfectSpecular* Clone() const;

	virtual RGBColor SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;
	virtual RGBColor SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

	inline void SetKr(float kr) {this->m_kr = kr;};
	inline void SetCr(const RGBColor& cr) {this->m_cr = cr;};

private:
	float m_kr;	// reflection coefficient
	RGBColor m_cr;	// reflection color
};

#endif