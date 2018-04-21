#ifndef __DIRECTION_H__
#define __DIRECTION_H__

#include "light.h"

class Direction: public Light
{
public:
	Direction();
	Direction(const Direction& p);
	Direction& operator= (const Direction& rhs);
	~Direction();

	virtual Light* clone() const;
	virtual Vector3D get_direction(ShadeRec& sr);
	virtual RGBColor L(ShadeRec& sr);
	inline void scale_radiance(const float ls) {this->ls = ls;};
	inline void set_color(const RGBColor& c) {this->color = c;};
	void set_direction(const Vector3D& d);
	virtual bool in_shadow(const Ray& ray, const ShadeRec& sr);

private:
	float ls;
	RGBColor color;
	Vector3D direction;
};

#endif