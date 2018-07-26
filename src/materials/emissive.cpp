#include "emissive.h"
#include "utilities/shaderec.h"

Emissive::Emissive()
	: ls(1.0f), ce(1.0f)
{

}

Emissive::Emissive(const Emissive& e)
	: ls(e.ls), ce(e.ce)
{
	
}

Material* Emissive::clone() const
{
	return new Emissive(*this);
}

Emissive::~Emissive()
{

}

Emissive& Emissive::operator =(const Emissive& rhs)
{
	if (&rhs == this)
		return *this;

	ls = rhs.ls;
	ce = rhs.ce;

	return *this;
}

RGBColor Emissive::shade(ShadeRec& sr)
{
	return area_light_shade(sr);
}

RGBColor Emissive::area_light_shade(ShadeRec& sr)
{
	if (sr.normal * -sr.ray.d > 0.0)
		return ls * ce;
	else
		return 0.0;
}

RGBColor Emissive::global_shade(ShadeRec& sr)
{
	if (sr.depth == 1)
		return 0.0;

	return area_light_shade(sr);
}