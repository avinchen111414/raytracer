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
	// 因为表示介质的BRDF和BTDF都需要特化的Reflector和Transmitter
	// 所以这里就直接使用子类类型，不使用普适性的基类类型了。
	FresnelReflector* m_fresnel_brdf;
	FresnelTransmitter* m_fresnel_btdf;
	RGBColor m_cf_in;
	RGBColor m_cf_out;

private:

};

#endif