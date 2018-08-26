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
	Vector3D wo(-sr.ray.d); 
	float cos_thetai = sr.normal * wo;  
	float eta = ior;

	// etaָ�������ɿ��������������ʣ����������Ե�ķ���һ�����ڲ�ָ���ⲿ������
	// cos_theta < 0.0f��˵�����������������ʽ��������etaҪȡһ������
	if (cos_thetai < 0.0) 
		eta = 1.0 / eta; 

	return (1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta) < 0.0);
}
