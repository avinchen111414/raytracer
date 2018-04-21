#include "light.h"
#include "utilities/ray.h"

Light::Light():
	shadows(true)
{

}

Light::Light(const Light& rhs):
	shadows(rhs.shadows)
{

}

Light& Light::operator= (const Light& rhs)
{
	if (this == &rhs) {
		return *this;
	}

	shadows = rhs.shadows;
	return *this;
}

Light::~Light()
{

}

bool Light::in_shadow(const Ray& ray, const ShadeRec& sr)
{
	return false;
}

float Light::G(const ShadeRec& sr) const
{
	return 1.0f;
}

float Light::pdf(const ShadeRec& sr) const
{
	return 1.0f;
}