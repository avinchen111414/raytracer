#ifndef __MESHTRIANGLE_H__
#define __MESHTRIANGLE_H__

#include "geometricobject.h"
#include "utilities/bbox.h"

class Mesh;
class Ray;
class ShadeRec;

class MeshTriangle: public GeometricObject
{
public:

	MeshTriangle(void);   	  									
	MeshTriangle(Mesh* _mesh_ptr, const int i1, const int i2, const int i3);		
	MeshTriangle(const MeshTriangle& mt); 					
	virtual	~MeshTriangle(void);   									
	MeshTriangle& operator= (const MeshTriangle& rhs);

	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const = 0;
	virtual	bool ShadowHit(const Ray& ray, float& tmin) const;
	virtual const BBox* GetBoundingBox() const;

	void ComputeNormal(const bool reverse_normal);
	void SetMesh(Mesh* mesh, const int i1, const int i2, const int i3);

	virtual Normal GetNormal() const;
	
protected:
	void update_bbox();

protected:

	Mesh* m_mesh;
	BBox m_bbox;

	int m_index0, m_index1, m_index2;
	Normal m_normal;
	float m_area;

};

#endif