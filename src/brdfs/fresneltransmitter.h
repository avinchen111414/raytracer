#ifndef __FRESNELTRANSMITTER_H__
#define __FRESNELTRANSMITTER_H__

#include "BTDF.h"

class FresnelTransmitter: public BTDF
{
public:
	FresnelTransmitter();

	FresnelTransmitter(const FresnelTransmitter& ft);

	virtual RGBColor f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi);
	virtual RGBColor rho(const ShadeRec& sr, const Vector3D& wo);
	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt);
	
	virtual FresnelTransmitter* clone() const;

	virtual bool tir(const ShadeRec& sr) const;

	void set_eta(float eta_in, float eta_out) {this->eta_in = eta_in; this->eta_out = eta_out;};

	float fresnel(const ShadeRec& sr) const;

protected:

private:
	float eta_in;
	float eta_out;
};

#endif