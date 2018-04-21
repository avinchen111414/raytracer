#include "utilities/shaderec.h"
#include "material.h"

Material::Material()
	:recv_shadow(true)
{

}

Material::Material(const Material& m)
	:recv_shadow(m.recv_shadow)
{

}

Material::~Material() {

}

RGBColor Material::area_light_shade(ShadeRec& sr)
{
	return 0.0f;
}

RGBColor Material::shade(ShadeRec& sr) {
	return 0.0f;
}

RGBColor Material::get_le(ShadeRec& sr) const {
	return 0.0f;
}

Material& Material::operator=(const Material& rhs) {
	if (&rhs == this)
		return *this;

	this->recv_shadow = rhs.recv_shadow;

	return *this;
}