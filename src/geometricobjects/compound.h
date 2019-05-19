#ifndef __COMPOUND_H__
#define __COMPOUND_H__

#include <vector>
#include "geometricobject.h"
#include "utilities/bbox.h"

class Compound: public GeometricObject
{
public:
	Compound();
	Compound(const Compound& object);
	Compound& operator= (const Compound& rhs);

	virtual ~Compound();
	virtual GeometricObject* Clone() const;
	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& s) const;
	virtual void SetMaterial(Material* m);
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;
	virtual const BBox* GetBoundingBox() const;
	
	virtual void AddObject(GeometricObject* object);
	size_t GetObjectNum() const;

protected:
	void UpdateBbox();
	void DeleteObjects();
	void CopyObjects(const std::vector<GeometricObject*>& rhs_objects);

protected:
	std::vector<GeometricObject*> m_objects;

private:
	BBox m_bbox;
};

#endif