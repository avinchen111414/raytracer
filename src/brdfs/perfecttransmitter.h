#ifndef __PERFECTTRANSMITTER_H__
#define __PERFECTTRANSMITTER_H__

#include "BTDF.h"

class PerfectTransmitter: public BTDF
{
public:
	PerfectTransmitter();
	PerfectTransmitter(const PerfectTransmitter& other);

	virtual RGBColor sample_f(const ShadeRec& sr, const Vector3D& wo,
		Vector3D& wt);

	virtual PerfectTransmitter* clone() const;
};

#endif