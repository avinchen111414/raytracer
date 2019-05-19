#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include "geometricobject.h"
#include "utilities/matrix.h"
#include "utilities/bbox.h"

class Ray;
class ShadeRec;
class Point3D;
class Vector3D;
class Instance: public GeometricObject
{
public:
	Instance();
	Instance(GeometricObject* object);
	Instance(const Instance& other);
	virtual ~Instance();

	Instance& operator= (const Instance& rhs);
	virtual GeometricObject* Clone() const;
	virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;
	virtual const BBox* GetBoundingBox() const;

	void SetObject(GeometricObject* object);
	
	// for area light
	virtual Point3D Sample();
	virtual float Pdf(const ShadeRec& sr);
	virtual Normal GetNormal(const Point3D& p);

	// affine transformation functions
	void Translate(const Vector3D& trans);
	void Translate(double dx, double dy, double dz);

	void Scale(const Vector3D& s);
	void Scale(double sx, double sy, double sz);

	void RotateX(double r);
	void RotateY(double r);
	void RotateZ(double r);

	void Shear(const Matrix& s);
	void EndTransform();

protected:
	void UpdateBbox();

private:
	GeometricObject* m_object;
	Matrix m_inv_matrix;
	Matrix m_forward_matrix;
	bool m_transform_the_texture;
	BBox m_bbox;
};

#endif