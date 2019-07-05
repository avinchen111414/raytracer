#include <cmath>
#include <algorithm>
#include "instance.h"
#include "utilities/ray.h"
#include "utilities/shaderec.h"
#include "utilities/point3d.h"
#include "utilities/vector3d.h"
#include "utilities/utility.h"
#include "materials/material.h"

Instance::Instance()
	: GeometricObject(),
	m_object(nullptr),
	m_bbox(Point3D(0), Point3D(0))
{
	m_inv_matrix.SetIDentity();
	m_forward_matrix.SetIDentity();
	m_material = nullptr;
}

Instance::Instance(GeometricObject* object)
	: GeometricObject(),
	m_object(object),
	m_bbox(Point3D(0), Point3D(0))
{
	m_inv_matrix.SetIDentity();
	m_forward_matrix.SetIDentity();
	if (object)
		m_material = object->GetMaterial();
}

Instance::Instance(const Instance& other)
	: GeometricObject(other),
	m_inv_matrix(other.m_inv_matrix),
	m_forward_matrix(other.m_forward_matrix),
	m_bbox(other.m_bbox)
{
	if (other.m_object)
		m_object = other.m_object->Clone();
	else
		m_object = nullptr;

	m_material = other.m_material;
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
		m_object = rhs.m_object->Clone();
	else
		m_object = nullptr;

	m_inv_matrix = rhs.m_inv_matrix;
	m_forward_matrix = rhs.m_forward_matrix;
	m_bbox = rhs.m_bbox;
	m_transform_the_texture = rhs.m_transform_the_texture;
	m_material = rhs.m_material;
	return *this;
}

GeometricObject* Instance::Clone() const
{
	return new Instance(*this);
}

bool Instance::Hit(const Ray& ray, double& t, ShadeRec& s) const
{
	Ray inv_ray(ray);
	inv_ray.o = m_inv_matrix * inv_ray.o;
	inv_ray.d = m_inv_matrix * inv_ray.d;

	if (m_object->Hit(inv_ray, t, s))
	{
		s.normal = m_inv_matrix * s.normal;
		s.normal.Normalize();
		s.local_hit_point = ray.o + t * ray.d;
		return true;
	}

	return false;
}

bool Instance::ShadowHit(const Ray& ray, float& tmin) const
{
	Ray inv_ray(ray);
	inv_ray.o = m_inv_matrix * inv_ray.o;
	inv_ray.d = m_inv_matrix * inv_ray.d;

	if (m_object->ShadowHit(inv_ray, tmin))
		return true;

	return false;
}

void Instance::SetObject(GeometricObject* object)
{
	if (object)
	{
		m_object = object->Clone();
		m_material = m_object->GetMaterial();
	}
	else
	{
		m_object = nullptr;
		m_material = nullptr;
	}
}

Point3D Instance::Sample()
{
	if (m_object)
		return m_object->Sample();
	return GeometricObject::Sample();
}

float Instance::Pdf(const ShadeRec& sr)
{
	if (m_object)
		return m_object->Pdf(sr);
	return GeometricObject::Pdf(sr);
}

Normal Instance::GetNormal(const Point3D& p)
{
	if (m_object)
		return m_object->GetNormal(p);
	return GeometricObject::GetNormal(p);
}

void Instance::Translate(const Vector3D& trans)
{
	Matrix inv_translation_matrix;
	inv_translation_matrix.m[0][3] = -trans.x;
	inv_translation_matrix.m[1][3] = -trans.y;
	inv_translation_matrix.m[2][3] = -trans.z;

	m_inv_matrix = m_inv_matrix * inv_translation_matrix;

	Matrix forward_translation_matrix;
	forward_translation_matrix.m[0][3] = trans.x;
	forward_translation_matrix.m[1][3] = trans.y;
	forward_translation_matrix.m[2][3] = trans.z;

	m_forward_matrix = forward_translation_matrix * m_forward_matrix;
}

void Instance::Translate(double dx, double dy, double dz)
{
	Translate(Vector3D(dx, dy, dz));
}

void Instance::RotateX(double r)
{
	double sin_theta = sin(r * PI_ON_180);
	double cos_theta = cos(r * PI_ON_180);
	
	Matrix inv_x_rotation_matrix;					// temporary inverse rotation matrix about x axis
	
	inv_x_rotation_matrix.m[1][1] = cos_theta;
	inv_x_rotation_matrix.m[1][2] = sin_theta;   	
	inv_x_rotation_matrix.m[2][1] = -sin_theta;  
	inv_x_rotation_matrix.m[2][2] = cos_theta;		
					
	m_inv_matrix = m_inv_matrix * inv_x_rotation_matrix;

	Matrix x_rotation_matrix;

	x_rotation_matrix.m[1][1] = cos_theta;
	x_rotation_matrix.m[1][2] = -sin_theta;
	x_rotation_matrix.m[2][1] = sin_theta;
	x_rotation_matrix.m[2][2] = cos_theta;

	m_forward_matrix = x_rotation_matrix * m_forward_matrix;

}

void Instance::RotateY(double r)
{
	double sin_theta = sin(r * PI / 180.0);
	double cos_theta = cos(r * PI / 180.0);

	Matrix inv_y_rotation_matrix;					// temporary inverse rotation matrix about y axis

	inv_y_rotation_matrix.m[0][0] = cos_theta;   
	inv_y_rotation_matrix.m[0][2] = -sin_theta;  
	inv_y_rotation_matrix.m[2][0] = sin_theta;
	inv_y_rotation_matrix.m[2][2] = cos_theta;		

	m_inv_matrix = m_inv_matrix * inv_y_rotation_matrix;

	Matrix y_rotation_matrix;

	y_rotation_matrix.m[0][0] = cos_theta;
	y_rotation_matrix.m[0][2] = sin_theta;
	y_rotation_matrix.m[2][0] = -sin_theta;
	y_rotation_matrix.m[2][2] = cos_theta;

	m_forward_matrix = y_rotation_matrix * m_forward_matrix;
}

void Instance::RotateZ(double r)
{
	double sin_theta = sin(r * PI / 180.0);
	double cos_theta = cos(r * PI / 180.0);

	Matrix inv_z_rotation_matrix;					// temporary inverse rotation matrix about y axis	

	inv_z_rotation_matrix.m[0][0] = cos_theta;
	inv_z_rotation_matrix.m[0][1] = sin_theta;   	
	inv_z_rotation_matrix.m[1][0] = -sin_theta;  
	inv_z_rotation_matrix.m[1][1] = cos_theta;	

	m_inv_matrix = m_inv_matrix * inv_z_rotation_matrix;

	Matrix z_rotation_matrix;

	z_rotation_matrix.m[0][0] = cos_theta;
	z_rotation_matrix.m[0][1] = -sin_theta;
	z_rotation_matrix.m[1][0] = sin_theta;
	z_rotation_matrix.m[1][1] = cos_theta;

	m_forward_matrix = z_rotation_matrix * m_forward_matrix;
}

void Instance::Shear(const Matrix& s)
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

	m_forward_matrix = s * m_forward_matrix;
}

void Instance::Scale(const Vector3D& s)
{
	Matrix	inv_scaling_matrix;			// temporary inverse scaling matrix

	inv_scaling_matrix.m[0][0] = 1.0 / s.x;
	inv_scaling_matrix.m[1][1] = 1.0 / s.y;
	inv_scaling_matrix.m[2][2] = 1.0 / s.z;

	m_inv_matrix = m_inv_matrix * inv_scaling_matrix;

	Matrix scaling_matrix;

	scaling_matrix.m[0][0] = s.x;
	scaling_matrix.m[1][1] = s.y;
	scaling_matrix.m[2][2] = s.z;

	m_forward_matrix = scaling_matrix * m_forward_matrix;
}

void Instance::Scale(double sx, double sy, double sz)
{
	Scale(Vector3D(sx, sy, sz));
}

void Instance::EndTransform()
{
	UpdateBbox();
}

void Instance::UpdateBbox()
{
	// 1. 首先拿到原物体的bbox
	const BBox* bbox = m_object->GetBoundingBox();
	if (!bbox)
		return;
	const BBox& object_bbox = *bbox;

	// 2. 对原物体的bbox实施Instance的所有齐次变换
	Point3D v[8];
	v[0].x = object_bbox.x0; v[0].y = object_bbox.y0; v[0].z = object_bbox.z0;
	v[1].x = object_bbox.x1; v[1].y = object_bbox.y0; v[1].z = object_bbox.z0;
	v[2].x = object_bbox.x1; v[2].y = object_bbox.y1; v[2].z = object_bbox.z0;
	v[3].x = object_bbox.x0; v[3].y = object_bbox.y1; v[3].z = object_bbox.z0;

	v[4].x = object_bbox.x0; v[4].y = object_bbox.y0; v[4].z = object_bbox.z1;
	v[5].x = object_bbox.x1; v[5].y = object_bbox.y0; v[5].z = object_bbox.z1;
	v[6].x = object_bbox.x1; v[6].y = object_bbox.y1; v[6].z = object_bbox.z1;
	v[7].x = object_bbox.x0; v[7].y = object_bbox.y1; v[7].z = object_bbox.z1;

	v[0] = m_forward_matrix * v[0];
	v[1] = m_forward_matrix * v[1];
	v[2] = m_forward_matrix * v[2];
	v[3] = m_forward_matrix * v[3];
	v[4] = m_forward_matrix * v[4];
	v[5] = m_forward_matrix * v[5];
	v[6] = m_forward_matrix * v[6];
	v[7] = m_forward_matrix * v[7];

	// 3. 因为bbox实施变换后，bbox的边可能不再与轴平行，这里取变换后的bbox的bounding作为近似
	double x0 = INT_MAX, y0 = INT_MAX, z0 = INT_MAX;
	for (int i = 0; i < 8; i++)
		x0 = std::min(x0, v[i].x);
	for (int i = 0; i < 8; i++)
		y0 = std::min(y0, v[i].y);
	for (int i = 0; i < 8; i++)
		z0 = std::min(z0, v[i].z);

	double x1 = -INT_MAX, y1 = -INT_MAX, z1 = -INT_MAX;
	for (int i = 0; i < 8; i++)
		x1 = std::max(x1, v[i].x);
	for (int i = 0; i < 8; i++)
		y1 = std::max(y1, v[i].y);
	for (int i = 0; i < 8; i++)
		z1 = std::max(z1, v[i].z);

	m_bbox.x0 = x0; m_bbox.y0 = y0; m_bbox.z0 = z0;
	m_bbox.x1 = x1; m_bbox.y1 = y1; m_bbox.z1 = z1;
}

const BBox* Instance::GetBoundingBox() const
{
	return &m_bbox;
}
