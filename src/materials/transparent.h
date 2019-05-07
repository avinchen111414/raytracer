#ifndef __TRANSPARENT_H__
#define __TRANSPARENT_H__

#include "phong.h"

class PerfectTransmitter;
class PerfectSpecular;
class Transparent: public Phong
{
public:
	Transparent();
	Transparent(const Transparent& other);
	virtual Material* Clone() const;
	Transparent& operator= (const Transparent& rhs);

	virtual RGBColor area_light_shade(ShadeRec& sr);

	void set_kr(float kr);
	void set_cr(const RGBColor& cr);
	void set_kt(float kt);
	void set_ior(float ior);
	
protected:

private:
	PerfectSpecular* reflective_brdf;
	PerfectTransmitter* transmitive_btdf;
};

#endif