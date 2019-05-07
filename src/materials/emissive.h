#ifndef __EMISSIVE_H__
#define __EMISSIVE_H__

#include "material.h"
#include "utilities/rgbcolor.h"

class ShadeRec;
class Emissive: public Material
{
private:
	float ls;
	RGBColor ce;

public:
	Emissive();
	Emissive(const Emissive& e);
	virtual Material* Clone() const;
	virtual ~Emissive();
	Emissive& operator= (const Emissive& rhs);

	virtual RGBColor shade(ShadeRec& sr);
	virtual RGBColor area_light_shade(ShadeRec& sr);
	virtual RGBColor global_shade(ShadeRec& sr);
	virtual void set_sampler(Sampler* sampler_ptr) {};

	RGBColor get_le(ShadeRec& sr) const { return ls * ce; }
	void set_ls(const float ls) { this->ls = ls; };
	void set_ce(const RGBColor& ce) {this->ce = ce; };

};

#endif