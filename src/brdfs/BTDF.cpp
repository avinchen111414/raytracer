#include "utilities/shaderec.h"
#include "utilities/vector3d.h"
#include "sampler/sampler.h"
#include "BTDF.h"

BTDF::BTDF():
	k(1.0f), ior(1.0f)
{

}

BTDF::BTDF(const BTDF& btdf_object):
	k(btdf_object.k), ior(btdf_object.ior)
{

}

RGBColor BTDF::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi)
{
	return RGBColor(0.0f);
}

RGBColor BTDF::sample_f(const ShadeRec& sr, const Vector3D& wo,
						Vector3D& wt)
{
	return RGBColor(0.0f);
}

RGBColor BTDF::rho(const ShadeRec& sr, const Vector3D& wo)
{
	return RGBColor(0.0f);
}

bool BTDF::tir(const ShadeRec& sr) const
{
	Vector3D wo = -sr.ray.d;
	float cos_theta = sr.normal * wo;
	float eta = ior;

	// eta指由射线由空气进入折射物质，而且物体边缘的法线一般由内部指向外部，所以
	// cos_theta < 0.0f，说明现在是由折射物质进入空气，eta要取一个倒数
	if (cos_theta < 0.0f)
		eta = 1.0f / eta;

	return 1.0f - 1.0f / (eta * eta) * (1.0f - cos_theta * cos_theta) < 0.0f;


}
