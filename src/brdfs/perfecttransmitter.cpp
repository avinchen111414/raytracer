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
	// 注意射线的方向与能量传输的方向是相反的（P.571）
	// 所以返回值中的eta * eta是作为分母
	Normal n(sr.normal);
	float cos_thetai = n * wo;
	float eta = ior;	

	if (cos_thetai < 0.0) {			// transmitted ray is outside     
		cos_thetai = -cos_thetai;
		n = -n;  					// reverse direction of normal
		eta = 1.0 / eta; 			// invert ior 
	}

	float temp = 1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta);
	float cos_theta2 = sqrt(temp);
	wt = -wo / eta - (cos_theta2 - cos_thetai / eta) * n;   

	return (k / (eta * eta) * RGBColor(1.0f) / fabs(sr.normal * wt));
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