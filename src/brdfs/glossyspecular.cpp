#include "glossyspecular.h"
#include "utilities/shaderec.h"
#include <cmath>

using namespace std;

GlossySpecular::GlossySpecular():
	ks(1.0),
	cs(1.0),
	exp(1.0)
{

}

GlossySpecular::GlossySpecular(const GlossySpecular& glossy):
	ks(glossy.ks),
	cs(glossy.cs),
	exp(glossy.exp)
{

}

GlossySpecular& GlossySpecular::operator=(const GlossySpecular& rhs)
{
	if (this == &rhs)
		return *this;

	BRDF::operator=(rhs);

	ks = rhs.ks;
	cs = rhs.cs;
	exp = rhs.exp;

	return *this;
}

GlossySpecular::~GlossySpecular()
{

}

GlossySpecular* GlossySpecular::clone() const
{
	return (new GlossySpecular(*this));
}

RGBColor GlossySpecular::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi)
{
	RGBColor L(0.0);
	float n_dot_wi = static_cast<float>(sr.normal * wi);
	Vector3D r = -wi + 2.0 * (sr.normal * n_dot_wi);
	float r_dot_wo = static_cast<float>(r * wo);

	if (r_dot_wo > 0.0)
		L = ks * cs * pow(r_dot_wo, exp);

	return L;
}