#include "point.h"
#include "shaderec.h"
#include "world.h"

Point::Point():
	Light(),
	ls(1.0),
	color(1.0),
	location(0.0)
{

}

Point::Point(const Point& p):
	Light(p),
	ls(p.ls),
	color(p.color),
	location(p.location)
{

}

Point& Point::operator=(const Point& rhs)
{
	if (this == &rhs)
		return *this;

	Light::operator=(rhs);

	ls = rhs.ls;
	color = rhs.color;
	location = rhs.location;
	return *this;
}

Point::~Point()
{

}

Light* Point::clone() const
{
	return new Point(*this);
}

Vector3D Point::get_direction(ShadeRec& sr)
{
	Vector3D dir = location - sr.hit_point;
	dir.normalize();
	return dir;
}

RGBColor Point::L(ShadeRec& sr)
{
	return ls*color;
}

bool Point::in_shadow(const Ray& ray, const ShadeRec& sr)
{
	float t;
	int num_objects = sr.w.objects.size();
	float d = static_cast<float>(location.distance(ray.o));

	for (int j = 0; j != num_objects; j++)
	{
		if (sr.w.objects[j]->shadow_hit(ray, t) && t < d)
		{
			return true;
		}
	}
	return false;
}