#ifndef __COMPOUND_H__
#define __COMPOUND_H__

#include "geometricobject.h"
#include <vector>

class Compound: public GeometricObject
{
public:
	Compound();
	Compound(const Compound& object);
	Compound& operator= (const Compound& rhs);

	virtual ~Compound();
	virtual GeometricObject* clone() const;
	virtual bool hit(const Ray& ray, double& tmin, ShadeRec& s) const;
	virtual void set_material(Material* m);
	virtual bool shadow_hit(const Ray& ray, float& tmin) const;
	
	void add_object(GeometricObject* object);
	size_t get_object_num() const;

protected:
	void delete_objects();
	void copy_objects(const std::vector<GeometricObject*>& rhs_objects);

protected:
	std::vector<GeometricObject*> m_objects;
};

#endif