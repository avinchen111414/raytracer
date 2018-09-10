#ifndef __DIELECTRIC_H__
#define __DIELECTRIC_H__

#include "phong.h"

class ShadeRec;
class FresnelReflector;
class FresnelTransmitter;

class Dielectric: public Phong
{
public:
	Dielectric();
	Dielectric(const Dielectric& dlt);
	virtual Material* clone() const;
	Dielectric& operator= (const Dielectric& rhs);

	virtual RGBColor area_light_shade(ShadeRec& sr);

	void set_eta(float eta_in, float eta_out);
	void set_color_filter(RGBColor cf_in, RGBColor cf_out);

protected:
	FresnelReflector* fresnel_brdf;
	FresnelTransmitter* fresnel_btdf;
	RGBColor cf_in;
	RGBColor cf_out;

private:

};

#endif