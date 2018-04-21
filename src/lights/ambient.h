#ifndef __AMBIENT_H__
#define __AMBIENT_H__

#include "light.h"

class RGBColor;

class Ambient: public Light
{
public:
	Ambient();
	Ambient(const Ambient& a);
	Ambient& operator= (const Ambient& rhs);
	~Ambient();

	virtual Light* clone() const;
	virtual Vector3D get_direction(ShadeRec& s);
	virtual RGBColor L(ShadeRec& s);

	void scale_radiance(const float b);
	void set_color(const RGBColor& c);

private:
	float ls;
	RGBColor color;
};

inline void Ambient::scale_radiance(const float b)
{
	ls = b;
}

inline void Ambient::set_color(const RGBColor& c)
{
	color = c;
}

#endif