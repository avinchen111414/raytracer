#ifndef __FRESNELTRANSMITTER_H__
#define __FRESNELTRANSMITTER_H__

#include "BTDF.h"

class FresnelTransmitter: public BTDF
{
public:
	FresnelTransmitter();

	FresnelTransmitter(const FresnelTransmitter& ft);

	virtual RGBColor F(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi);
	virtual RGBColor Rho(const ShadeRec& sr, const Vector3D& wo);
	virtual RGBColor SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt);
	
	virtual FresnelTransmitter* Clone() const;

	virtual bool Tir(const ShadeRec& sr) const;

	void SetEta(float eta_in, float eta_out) {this->m_eta_in = eta_in; this->m_eta_out = eta_out;};

	float Fresnel(const ShadeRec& sr) const;

protected:

private:
	float m_eta_in;
	float m_eta_out;
};

#endif