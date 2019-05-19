#include "utilities/shaderec.h"
#include "utilities/vector3d.h"
#include "utilities/normal.h"
#include "sampler/sampler.h"
#include "fresneltransmitter.h"

FresnelTransmitter::FresnelTransmitter()
	: BTDF(), m_eta_in(1.0f), m_eta_out(1.0f)
{}

FresnelTransmitter::FresnelTransmitter(const FresnelTransmitter& ft)
	: BTDF(ft), m_eta_in(ft.m_eta_in), m_eta_out(ft.m_eta_out)
{}

RGBColor FresnelTransmitter::SampleF(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt)
{
	// ע�����ߵķ�������������ķ������෴�ģ�P.571��
	// ���Է���ֵ�е�eta * eta����Ϊ��ĸ
	Normal n(sr.normal);
	float cos_thetai = n * wo;
	float eta = m_eta_in / m_eta_out;	

	if (cos_thetai < 0.0) {			// transmitted ray is outside     
		cos_thetai = -cos_thetai;
		n = -n;  					// reverse direction of normal
		eta = 1.0 / eta; 			// invert ior 
	}

	float temp = 1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta);
	float cos_theta2 = sqrt(temp);
	wt = -wo / eta - (cos_theta2 - cos_thetai / eta) * n;   

	return (Fresnel(sr) / (eta * eta) * RGBColor(1.0f) / fabs(sr.normal * wt));
}

FresnelTransmitter* FresnelTransmitter::Clone() const
{
	return new FresnelTransmitter(*this);
}

RGBColor FresnelTransmitter::F(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi)
{
	return 0.0f;
}

RGBColor FresnelTransmitter::Rho(const ShadeRec& sr, const Vector3D& wo)
{
	return 0.0f;
}

bool FresnelTransmitter::Tir(const ShadeRec& sr) const
{
	Vector3D wo(-sr.ray.d); 
	float cos_thetai = sr.normal * wo;  
	float eta = m_eta_in / m_eta_out;

	// etaָ�������ɿ��������������ʣ����������Ե�ķ���һ�����ڲ�ָ���ⲿ������
	// cos_theta < 0.0f��˵�����������������ʽ��������etaҪȡһ������
	if (cos_thetai < 0.0) 
		eta = 1.0 / eta; 

	return (1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta) < 0.0);
}

float FresnelTransmitter::Fresnel(const ShadeRec& sr) const
{
	Normal normal(sr.normal);
	float ndotd = normal * -sr.ray.d;

	float eta = 0.0f;
	// Ray hits inside surface
	if (ndotd < 0.0f)
	{
		normal = -normal;
		eta = m_eta_out / m_eta_in;
	}
	else
		eta = m_eta_in / m_eta_out;

	float cos_theta_i = normal * -sr.ray.d;
	float cos_theta_t = sqrt(1.0f - (1.0f - cos_theta_i * cos_theta_i) / (eta * eta));

	float r_parallel = (eta * cos_theta_i - cos_theta_t) / 
		(eta * cos_theta_i + cos_theta_t);
	float r_perpendicular = (cos_theta_i - eta * cos_theta_t) /
		(cos_theta_i + eta * cos_theta_t);
	float kr = 0.5 * (r_parallel * r_parallel +
		r_perpendicular * r_perpendicular);
	return 1.0f - kr;
}