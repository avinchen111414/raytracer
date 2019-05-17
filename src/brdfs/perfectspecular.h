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

	virtual PerfectSpecular* clone() const;

	virtual RGBColor SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;
	virtual RGBColor SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

	inline void set_kr(float kr) {this->kr = kr;};
	inline void set_cr(const RGBColor& cr) {this->cr = cr;};

private:
	float kr;	// reflection coefficient
	RGBColor cr;	// reflection color
};

#endif