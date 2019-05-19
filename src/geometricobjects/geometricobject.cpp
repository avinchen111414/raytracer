#include "GeometricObject.h"

const double GeometricObject::m_eplison = 0.000001;

GeometricObject::GeometricObject(void)
	: m_color(0.0), m_shadow(true), m_material(nullptr)
{}

GeometricObject::GeometricObject (const GeometricObject& object) {
	m_color = object.m_color;
	m_shadow = object.m_shadow;
	m_material = object.m_material;
}	

GeometricObject& GeometricObject::operator= (const GeometricObject& rhs) {
	if (this == &rhs)
		return (*this);

	m_color = rhs.m_color;
	m_shadow = rhs.m_shadow;
	m_material = rhs.m_material;
	return (*this);
}

GeometricObject::~GeometricObject (void) {	
}

void GeometricObject::SetColor(const RGBColor& color)
{
	m_color = color;
}

void GeometricObject::SetMaterial(Material* m)
{
	m_material = m;
}

bool GeometricObject::ShadowHit(const Ray& ray, float& tmin) const
{
	return false;
}

Point3D GeometricObject::Sample()
{
	throw "Calling GeometricObject::sample does not make sense";
	return Point3D(0.0f);
}

float GeometricObject::Pdf(const ShadeRec& sr)
{
	throw "Calling GeometricObject::pdf does not make sense";
	return 0.0f;
}

Normal GeometricObject::GetNormal(const Point3D& p) const
{
	throw "Calling GeometricObject::get_normal does not make sense";
	return Normal(0.0f);
}

Normal GeometricObject::GetNormal() const
{
	throw "Calling GeometricObject::get_normal does not make sense";
	return Normal(0.0f);
}

const BBox* GeometricObject::GetBoundingBox() const
{
	return nullptr;
}

