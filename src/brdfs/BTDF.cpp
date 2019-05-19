#include "utilities/shaderec.h"
#include "utilities/vector3d.h"
#include "sampler/sampler.h"
#include "BTDF.h"

BTDF::BTDF():
	m_k(1.0f), m_ior(1.0f)
{

}

BTDF::BTDF(const BTDF& btdf_object):
	m_k(btdf_object.m_k), m_ior(btdf_object.m_ior)
{

}

RGBColor BTDF::F(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi)
{
	return RGBColor(0.0f);
}

RGBColor BTDF::SampleF(const ShadeRec& sr, const Vector3D& wo,
						Vector3D& wt)
{
	return RGBColor(0.0f);
}

RGBColor BTDF::Rho(const ShadeRec& sr, const Vector3D& wo)
{
	return RGBColor(0.0f);
}

bool BTDF::Tir(const ShadeRec& sr) const
{
	Vector3D wo(-sr.ray.d); 
	float cos_thetai = sr.normal * wo;  
	float eta = m_ior;

	// etaָ�������ɿ��������������ʣ����������Ե�ķ���һ�����ڲ�ָ���ⲿ������
	// cos_theta < 0.0f��˵�����������������ʽ��������etaҪȡһ������
	if (cos_thetai < 0.0) 
		eta = 1.0 / eta; 

	return (1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta) < 0.0);
}
