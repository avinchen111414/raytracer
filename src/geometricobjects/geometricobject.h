#ifndef __GEOMETRIC_OBJECT__
#define __GEOMETRIC_OBJECT__

class Ray;
class ShadeRec;
class BBox;

#include "utilities/rgbcolor.h"
#include "utilities/point3d.h"
#include "utilities/normal.h"

class Material;

class GeometricObject {	
	
	public:	

		GeometricObject(void);									
		GeometricObject(const GeometricObject& object);	
		virtual ~GeometricObject (void);
	
		virtual GeometricObject*								
		Clone(void) const = 0;

		virtual bool 												 
		Hit(const Ray& ray, double& t, ShadeRec& s) const = 0;
				
		RGBColor GetColor(void) const;
		virtual void SetColor(const RGBColor& color); 

		virtual Material* GetMaterial() const;
		virtual void SetMaterial(Material* m);

		virtual bool ShadowHit(const Ray& ray, float& tmin) const;
		
		inline void EnableShadow(bool enable) {m_shadow = enable;};

		virtual const BBox* GetBoundingBox() const;

		// for area light
		virtual Point3D Sample();
		virtual float Pdf(const ShadeRec& sr);
		virtual Normal GetNormal(const Point3D& p) const;
		virtual Normal GetNormal() const;
	
	protected:
		RGBColor m_color;
		Material* m_material;
		bool m_shadow;
	
		GeometricObject&				
		operator= (const GeometricObject& rhs);

		static const double m_eplison;
};

inline RGBColor GeometricObject::GetColor(void) const {
	return m_color;
}

inline Material* GeometricObject::GetMaterial() const
{
	return m_material;
}

#endif