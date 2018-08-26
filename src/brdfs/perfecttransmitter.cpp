#include "utilities/shaderec.h"
#include "utilities/vector3d.h"
#include "utilities/normal.h"
#include "sampler/sampler.h"
#include "perfecttransmitter.h"

PerfectTransmitter::PerfectTransmitter():
	BTDF()
{

}

PerfectTransmitter::PerfectTransmitter(const PerfectTransmitter& other):
	BTDF(other)
{

}

RGBColor PerfectTransmitter::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt)
{
	// 注意射线的方向与能量传输的方向是相反的（P.571
	// 所以返回值中的eta * eta是作为分母
	Normal n = sr.normal;
	float cos_theta = n * wo;
	float eta = ior;

	if (cos_theta < 0.0f)
	{
		n = -sr.normal;
		cos_theta = -cos_theta;
		eta = 1.0f / eta;
	}

	float cos_theta_t_pow = 1.0f - 1.0f / (eta * eta) * (1.0f - cos_theta * cos_theta);
	float cos_theta_t = sqrt(cos_theta_t_pow);
	wt = -wo / eta - (cos_theta_t - cos_theta / eta) * sr.normal;

	return k / (eta * eta) / fabs(sr.normal * wt);
	
}

PerfectTransmitter* PerfectTransmitter::clone() const
{
	return new PerfectTransmitter(*this);
}

RGBColor PerfectTransmitter::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi)
{
	return 0.0f;
}

RGBColor PerfectTransmitter::rho(const ShadeRec& sr, const Vector3D& wo)
{
	return 0.0f;
}