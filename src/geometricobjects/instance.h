#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include "geometricobject.h"
#include "utilities/matrix.h"

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
	virtual GeometricObject* clone() const;
	virtual bool hit(const Ray& ray, double& t, ShadeRec& s) const;
	virtual bool shadow_hit(const Ray& ray, float& tmin) const;

	void set_object(GeometricObject* object);
	
	// for area light
	virtual Point3D sample();
	virtual float pdf(const ShadeRec& sr);
	virtual Normal get_normal(const Point3D& p);

	// affine transformation functions
	void translate(const Vector3D& trans);
	void translate(double dx, double dy, double dz);

	void scale(const Vector3D& s);
	void scale(double sx, double sy, double sz);

	void rotate_x(double r);
	void rotate_y(double r);
	void rotate_z(double r);

	void shear(const Matrix& s);

protected:

private:
	GeometricObject* m_object;
	Matrix m_inv_matrix;
	bool transform_the_texture;
};

#endif