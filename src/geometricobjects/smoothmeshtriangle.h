#ifndef __SMOOTHMESHTRIANGLE_H__
#define __SMOOTHMESHTRIANGLE_H__

#include "flatmeshtriangle.h"
#include "utilities/normal.h"

class SmoothMeshTriangle: public FlatMeshTriangle
{
public:
	SmoothMeshTriangle(const SmoothMeshTriangle& other);
	SmoothMeshTriangle(Mesh* _mesh_ptr, const int i1, const int i2, const int i3);
	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

protected:
	Normal InterpolateNormal(const float beta, const float gamma) const;
	Normal m_n0, m_n1, m_n2;
};

#endif