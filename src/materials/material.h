#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "utilities/rgbcolor.h"

class ShadeRec;
class Sampler;
class Material
{
public:
	Material();
	Material(const Material& m);
	virtual Material* Clone() const = 0;
	virtual ~Material();
	
	virtual RGBColor shade(ShadeRec& sr);
	virtual RGBColor AreaLightShade(ShadeRec& sr);
	virtual RGBColor global_shade(ShadeRec& sr);

	inline void enable_recv_shadow(bool enable) {recv_shadow = enable;};
	virtual RGBColor get_le(ShadeRec& sr) const;
	virtual void set_sampler(Sampler* sampler_ptr) = 0;

protected:
	Material& operator= (const Material& rhs);
	bool recv_shadow;
};

#endif