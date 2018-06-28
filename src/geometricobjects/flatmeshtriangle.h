#ifndef __FLATMESHTRIANGLE_H__
#define __FLATMESHTRIANGLE_H__

#include "meshtriangle.h"

class FlatMeshTriangle: public MeshTriangle
{
public:
	FlatMeshTriangle(const FlatMeshTriangle& other);
	virtual GeometricObject* clone() const;
	virtual bool hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

protected:

private:

};

#endif