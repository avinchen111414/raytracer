#include "direction.h"
#include "utilities/shaderec.h"
#include "world/world.h"

Direction::Direction():
	Light(),
	ls(1.0),
	color(1.0),
	direction(Vector3D(0, 1, 0))
{

}

Direction::Direction(const Direction& d):
	Light(d),
	ls(d.ls),
	color(d.color),
	direction(d.direction)
{

}

Direction& Direction::operator=(const Direction& rhs)
{
	if (this == &rhs)
		return *this;

	Light::operator=(rhs);

	ls = rhs.ls;
	color = rhs.color;
	direction = rhs.direction;
	return *this;
}

Direction::~Direction()
{

}

Light* Direction::clone() const
{
	return new Direction(*this);
}

Vector3D Direction::get_direction(ShadeRec& sr)
{
	Vector3D dir = direction;
	return -dir.hat();
}

RGBColor Direction::L(ShadeRec& sr)
{
	return ls*color;
}

void Direction::set_direction(const Vector3D& d)
{
	direction = d;
}

bool Direction::in_shadow(const Ray& ray, const ShadeRec& sr)
{
	float t;
	int num_objects = sr.w.objects.size();

	for (int j = 0; j != num_objects; j++)
	{
		if (sr.w.objects[j]->ShadowHit(ray, t))
		{
			return true;
		}
	}
	return false;
}