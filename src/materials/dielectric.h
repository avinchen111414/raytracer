#ifndef __DIELECTRIC_H__
#define __DIELECTRIC_H__

#include "phong.h"

class ShadeRec;
class FresnelReflector;
class FresnelTransmitter;

class Dielectric: public Phong
{
public:
	Dielectric();
	Dielectric(const Dielectric& dlt);
	virtual Material* Clone() const;
	Dielectric& operator= (const Dielectric& rhs);

	virtual RGBColor AreaLightShade(ShadeRec& sr);

	void SetEta(float eta_in, float eta_out);
	void SetColorFilter(RGBColor cf_in, RGBColor cf_out);

protected:
	// ��Ϊ��ʾ���ʵ�BRDF��BTDF����Ҫ�ػ���Reflector��Transmitter
	// ���������ֱ��ʹ���������ͣ���ʹ�������ԵĻ��������ˡ�
	FresnelReflector* m_fresnel_brdf;
	FresnelTransmitter* m_fresnel_btdf;
	RGBColor m_cf_in;
	RGBColor m_cf_out;

private:

};

#endif