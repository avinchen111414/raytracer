#include "triangle.h"

Triangle::Triangle(const Point3D& _v0, const Point3D& _v1, const Point3D& _v2)
	: GeometricObject(), v0(_v0), v1(_v1), v2(_v2)
{
	normal = (v1 - v0) ^ (v2 - v0);
}

Triangle::Triangle(const Triangle& other)
	: GeometricObject(other), 
	v0(other.v0), v1(other.v1), v2(other.v2), normal(other.normal)
{

}

Triangle& Triangle::operator= (const Triangle& rhs)
{
	if (this == &rhs)
		return *this;
	
	GeometricObject::operator=(rhs);
	v0 = rhs.v0; v1 = rhs.v1; v2 = rhs.v2;
	normal = rhs.normal;

	return *this;
}

Triangle::~Triangle()
{

};

GeometricObject* Triangle::clone() const
{
	return new Triangle(*this);
}