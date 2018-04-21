#ifndef __POINT_H__
#define __POINT_H__

#include "light.h"
#include "point3d.h"

class Point: public Light
{
public:
	Point();
	Point(const Point& p);
	Point& operator= (const Point& rhs);
	~Point();

	virtual Light* clone() const;
	virtual Vector3D get_direction(ShadeRec& sr);
	virtual RGBColor L(ShadeRec& sr);

	void set_location(const Point3D& l);
	void scale_radiance(const float b);
	void set_color(const RGBColor& c);

	virtual bool in_shadow(const Ray& ray, const ShadeRec& sr);
private:
	float ls;
	RGBColor color;
	Point3D location;
};

inline void Point::set_location(const Point3D& l)
{
	location = l;
}

inline void Point::scale_radiance(const float b)
{
	ls = b;
}

inline void Point::set_color(const RGBColor& c)
{
	color = c;
}

#endif