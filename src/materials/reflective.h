#ifndef __REFLECTIVE_H__
#define __REFLECTIVE_H__

#include "phong.h"

class PerfectSpecular;
class Reflective: public Phong
{
public:
	Reflective();
	Reflective(const Reflective& r);
	virtual Material* clone() const;
	Reflective& operator= (const Reflective& rhs);

	virtual RGBColor area_light_shade(ShadeRec& sr);

	void set_kr(const float kr);
	void set_cr(const float cr);

protected:

private:
	PerfectSpecular* reflective_brdf;
};

#endif