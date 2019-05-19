#ifndef __FLATMESHTRIANGLE_H__
#define __FLATMESHTRIANGLE_H__

#include "meshtriangle.h"

class FlatMeshTriangle: public MeshTriangle
{
public:
	FlatMeshTriangle(const FlatMeshTriangle& other);
	FlatMeshTriangle(Mesh* _mesh_ptr, const int i1, const int i2, const int i3);
	virtual GeometricObject* Clone() const;
	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

protected:

private:

};

#endif