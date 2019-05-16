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

	virtual FresnelReflector* clone() const;

	virtual RGBColor FresnelReflector::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;

	float fresnel(const ShadeRec& sr) const;

	inline void set_eta(float eta_in, float eta_out) {this->eta_in = eta_in; this->eta_out = eta_out;};
protected:

private:
	RGBColor cr;
	float eta_in;
	float eta_out;

};

#endif