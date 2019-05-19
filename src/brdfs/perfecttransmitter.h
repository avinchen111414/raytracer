#ifndef __PERFECTTRANSMITTER_H__
#define __PERFECTTRANSMITTER_H__

#include "BTDF.h"

class PerfectTransmitter: public BTDF
{
public:
	PerfectTransmitter();
	PerfectTransmitter(const PerfectTransmitter& other);

	virtual RGBColor F(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi);
	virtual RGBColor Rho(const ShadeRec& sr, const Vector3D& wo);
	virtual RGBColor SampleF(const ShadeRec& sr, const Vector3D& wo,
		Vector3D& wt);

	virtual PerfectTransmitter* Clone() const;
};

#endif