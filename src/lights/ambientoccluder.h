#ifndef __AMBIENTOCCLUDER_H__
#define __AMBIENTCOOLUDER_H__

#include "light.h"

class Sampler;

class AmbientOccluder: public Light
{
public:
	AmbientOccluder();
	AmbientOccluder(const AmbientOccluder& ao);
	AmbientOccluder& operator= (const AmbientOccluder& rhs);
	~AmbientOccluder();

	virtual Light* clone() const;
	virtual Vector3D get_direction(ShadeRec& rc);
	virtual RGBColor L(ShadeRec& sr);
	void set_sampler(Sampler* sp);
	bool in_shadow(const Ray& ray, const ShadeRec& sr);
	inline void set_radiance(const float b) {ls = b;};
	inline void set_color(const RGBColor& c) {color = c;};
	inline void set_min_amount(const RGBColor& ma) {min_amount = ma;};
private:

	Sampler* sampler_ptr;
	RGBColor min_amount;
	float ls;
	RGBColor color;
};

#endif