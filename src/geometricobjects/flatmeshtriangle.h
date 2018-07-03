#ifndef __FLATMESHTRIANGLE_H__
#define __FLATMESHTRIANGLE_H__

#include "meshtriangle.h"

class FlatMeshTriangle: public MeshTriangle
{
public:
	FlatMeshTriangle(const FlatMeshTriangle& other);
	FlatMeshTriangle(Mesh* _mesh_ptr, const int i1, const int i2, const int i3);
	virtual GeometricObject* clone() const;
	virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool shadow_hit(const Ray& ray, float& tmin) const;

protected:

private:

};

#endif