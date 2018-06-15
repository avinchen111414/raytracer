#include <cmath>
#include "instance.h"
#include "utilities/ray.h"
#include "utilities/shaderec.h"
#include "utilities/point3d.h"
#include "utilities/vector3d.h"
#include "utilities/utility.h"
#include "materials/material.h"

Instance::Instance()
	: GeometricObject(),
	m_object(nullptr)
{
	m_inv_matrix.set_identity();
	m_material = nullptr;
}

Instance::Instance(GeometricObject* object)
	: GeometricObject(),
	m_object(object)
{
	m_inv_matrix.set_identity();
	if (object)
		m_material = object->get_material();
}

Instance::Instance(const Instance& other)
	: GeometricObject(other),
	m_inv_matrix(other.m_inv_matrix)
{
	if (other.m_object)
		m_object = other.m_object->clone();
	else
		m_object = nullptr;

	if (m_object)
		m_material = m_object->get_material();
}

Instance::~Instance()
{
	if (m_object)
	{
		delete m_object;
		m_object = nullptr;
	}
	m_material = nullptr;
}

Instance& Instance::operator=(const Instance& rhs)
{
	if (this == &rhs)
		return *this;

	if (m_object)
		delete m_object;
	if (rhs.m_object)
		m_object = rhs.m_object->clone();
	else
		m_object = nullptr;

	if (m_object)
		m_material = m_object->get_material();
	else
		m_material = nullptr;

	m_inv_matrix = rhs.m_inv_matrix;
	transform_the_texture = rhs.transform_the_texture;
	return *this;
}

GeometricObject* Instance::clone() const
{
	return new Instance(*this);
}

bool Instance::hit(const Ray& ray, double& t, ShadeRec& s) const
{
	Ray inv_ray(ray);
	inv_ray.o = m_inv_matrix * inv_ray.o;
	inv_ray.d = m_inv_matrix * inv_ray.d;

	if (m_object->hit(inv_ray, t, s))
	{
		s.normal = m_inv_matrix * s.normal;
		s.normal.normalize();
		s.local_hit_point = ray.o + t * ray.d;
		return true;
	}

	return false;
}

bool Instance::shadow_hit(const Ray& ray, float& tmin) const
{
	Ray inv_ray(ray);
	inv_ray.o = m_inv_matrix * inv_ray.o;
	inv_ray.d = m_inv_matrix * inv_ray.d;

	if (m_object->shadow_hit(inv_ray, tmin))
		return true;

	return false;
}

void Instance::set_object(GeometricObject* object)
{
	if (object)
	{
		m_object = object->clone();
		m_material = m_object->get_material();
	}
	else
	{
		m_object = nullptr;
		m_material = nullptr;
	}
}

Point3D Instance::sample()
{
	if (m_object)
		return m_object->sample();
	return GeometricObject::sample();
}

float Instance::pdf(const ShadeRec& sr)
{
	if (m_object)
		return m_object->pdf(sr);
	return GeometricObject::pdf(sr);
}

Normal Instance::get_normal(const Point3D& p)
{
	if (m_object)
		return m_object->get_normal(p);
	return GeometricObject::get_normal(p);
}

void Instance::translate(const Vector3D& trans)
{
	Matrix inv_translation_matrix;
	inv_translation_matrix.m[0][3] = -trans.x;
	inv_translation_matrix.m[1][3] = -trans.y;
	inv_translation_matrix.m[2][3] = -trans.z;

	m_inv_matrix = m_inv_matrix * inv_translation_matrix;
}

void Instance::translate(double dx, double dy, double dz)
{
	translate(Vector3D(dx, dy, dz));
}

void Instance::rotate_x(double r)
{
	double sin_theta = sin(r * PI_ON_180);
	double cos_theta = cos(r * PI_ON_180);
	
	Matrix inv_x_rotation_matrix;					// temporary inverse rotation matrix about x axis
	
	inv_x_rotation_matrix.m[1][1] = cos_theta;
	inv_x_rotation_matrix.m[1][2] = sin_theta;   	
	inv_x_rotation_matrix.m[2][1] = -sin_theta;  
	inv_x_rotation_matrix.m[2][2] = cos_theta;		
					
	m_inv_matrix = m_inv_matrix * inv_x_rotation_matrix;	
}

void Instance::rotate_y(double r)
{
	double sin_theta = sin(r * PI / 180.0);
	double cos_theta = cos(r * PI / 180.0);

	Matrix inv_y_rotation_matrix;					// temporary inverse rotation matrix about y axis

	inv_y_rotation_matrix.m[0][0] = cos_theta;   
	inv_y_rotation_matrix.m[0][2] = -sin_theta;  
	inv_y_rotation_matrix.m[2][0] = sin_theta;
	inv_y_rotation_matrix.m[2][2] = cos_theta;		

	m_inv_matrix = m_inv_matrix * inv_y_rotation_matrix;
}

void Instance::rotate_z(double r)
{
	double sin_theta = sin(r * PI / 180.0);
	double cos_theta = cos(r * PI / 180.0);

	Matrix inv_z_rotation_matrix;					// temporary inverse rotation matrix about y axis	

	inv_z_rotation_matrix.m[0][0] = cos_theta;
	inv_z_rotation_matrix.m[0][1] = sin_theta;   	
	inv_z_rotation_matrix.m[1][0] = -sin_theta;  
	inv_z_rotation_matrix.m[1][1] = cos_theta;	

	m_inv_matrix = m_inv_matrix * inv_z_rotation_matrix;
}

void Instance::shear(const Matrix& s)
{
	Matrix inverse_shearing_matrix;    // inverse shear matrix

	// discriminant

	double d = 1.0 	- s.m[1][0] * s.m[0][1] - s.m[2][0] * s.m[0][2]  - s.m[2][1] * s.m[1][2]
	+ s.m[1][0] * s.m[2][1] * s.m[0][2] + s.m[2][0] * s.m[0][1] * s.m[2][1];

	// diagonals

	inverse_shearing_matrix.m[0][0] = 1.0 - s.m[2][1] * s.m[1][2];
	inverse_shearing_matrix.m[1][1] = 1.0 - s.m[2][0] * s.m[0][2];
	inverse_shearing_matrix.m[2][2] = 1.0 - s.m[1][0] * s.m[0][1];
	inverse_shearing_matrix.m[3][3] = d;

	// first row

	inverse_shearing_matrix.m[0][1] = -s.m[1][0] + s.m[2][0] * s.m[1][2];
	inverse_shearing_matrix.m[0][2] = -s.m[2][0] + s.m[1][0] * s.m[2][1];

	// second row

	inverse_shearing_matrix.m[1][0] = -s.m[0][1] + s.m[2][1] * s.m[0][2];
	inverse_shearing_matrix.m[1][2] = -s.m[2][1] + s.m[2][0] * s.m[0][1];

	// third row

	inverse_shearing_matrix.m[2][0] = -s.m[0][2] + s.m[0][1] * s.m[1][2];
	inverse_shearing_matrix.m[2][1] = -s.m[1][2] + s.m[1][0] * s.m[0][2] ;

	// divide by discriminant

	inverse_shearing_matrix = inverse_shearing_matrix / d;

	m_inv_matrix = m_inv_matrix * inverse_shearing_matrix;	
}

void Instance::scale(const Vector3D& s)
{
	Matrix	inv_scaling_matrix;			// temporary inverse scaling matrix

	inv_scaling_matrix.m[0][0] = 1.0 / s.x;
	inv_scaling_matrix.m[1][1] = 1.0 / s.y;
	inv_scaling_matrix.m[2][2] = 1.0 / s.z;

	m_inv_matrix = m_inv_matrix * inv_scaling_matrix;
}

void Instance::scale(double sx, double sy, double sz)
{
	scale(Vector3D(sx, sy, sz));
}
