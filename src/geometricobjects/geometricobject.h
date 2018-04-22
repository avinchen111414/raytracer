#ifndef __GEOMETRIC_OBJECT__
#define __GEOMETRIC_OBJECT__

class Ray;
class ShadeRec;

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
		clone(void) const = 0;

		virtual bool 												 
		hit(const Ray& ray, double& t, ShadeRec& s) const = 0;
				
		RGBColor get_color(void) const;
		virtual void set_color(const RGBColor& color); 

		Material* get_material() const;
		virtual void set_material(Material* m);

		virtual bool shadow_hit(const Ray& ray, float& tmin) const;
		
		inline void enable_shadow(bool enable) {m_shadow = enable;};

		// for area light
		virtual Point3D sample();
		virtual float pdf(const ShadeRec& sr);
		virtual Normal get_normal(const Point3D& p);
	
	protected:
		RGBColor m_color;
		Material* m_material;
		bool m_shadow;
	
		GeometricObject&				
		operator= (const GeometricObject& rhs);

		static const double m_eplison;
};

inline RGBColor GeometricObject::get_color(void) const {
	return m_color;
}

inline Material* GeometricObject::get_material() const
{
	return m_material;
}

#endif