#ifndef __GLOSSYREFLECTIVE_H__
#define __GLOSSYREFLECTIVE_H__

#include "phong.h"

class GlossySpecular;
class Sampler;
class GlossyReflective: public Phong
{
public:
	GlossyReflective();
	GlossyReflective(const GlossyReflective& r);
	virtual Material* clone() const;
	GlossyReflective& operator= (const GlossyReflective& rhs);
	virtual void set_sampler(Sampler* sampler);

	virtual RGBColor area_light_shade(ShadeRec& sr);
	virtual RGBColor global_shade(ShadeRec& sr);

	void set_kr(const float kr);
	void set_cr(const float cr);
	void set_exp(const float exp);

protected:

private:
	GlossySpecular* glossy_reflective_brdf;
};

#endif