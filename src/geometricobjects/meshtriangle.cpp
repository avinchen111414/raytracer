#include "meshtriangle.h"
#include "utilities/mesh.h"
#include "utilities/ray.h"
#include "utilities/shaderec.h"
#include "utilities/utility.h"

MeshTriangle::MeshTriangle():
	GeometricObject(),
	m_mesh(nullptr),
	m_index0(0), m_index1(0), m_index2(0),
	m_normal(0), m_area(0)
{
	update_bbox();
}

MeshTriangle::MeshTriangle(Mesh* _mesh_ptr, const int i1, const int i2, const int i3):
	GeometricObject(),
	m_mesh(_mesh_ptr),
	m_index0(i1), m_index1(i2), m_index2(i3)
{
	compute_normal(false);
	update_bbox();
}

MeshTriangle::MeshTriangle(const MeshTriangle& mt):
	GeometricObject(mt),
	m_mesh(mt.m_mesh),
	m_index0(mt.m_index0), m_index1(mt.m_index1), m_index2(mt.m_index2),
	m_normal(mt.m_normal), m_area(mt.m_area)
{
	update_bbox();
}

MeshTriangle::~MeshTriangle()
{
	m_mesh = nullptr;
}

MeshTriangle& MeshTriangle::operator=(const MeshTriangle& rhs)
{
	if (this == &rhs)
		return *this;

	GeometricObject::operator=(rhs);
	m_mesh = rhs.m_mesh;
	m_index0 = rhs.m_index0;
	m_index1 = rhs.m_index1;
	m_index2 = rhs.m_index2;
	m_normal = rhs.m_normal;
	m_area = rhs.m_area;
	m_bbox = rhs.m_bbox;

	return *this;
}

// Excatlly same as Triangle::shadow_hit
bool MeshTriangle::shadow_hit(const Ray& ray, double& tmin) const
{
	const Point3D& v0 = m_mesh->vertices[m_index0];
	const Point3D& v1 = m_mesh->vertices[m_index1];
	const Point3D& v2 = m_mesh->vertices[m_index2];

	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x; 
	double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;

	double inv_denom  = 1.0 / (a * m + b * q + c * s);

	// beta =	|d b c|				D = |a b c|
	//			|h f g| D_inv			|e f g|
	//			|l j k|					|i j k|

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
		return (false);

	double r = r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0 )
		return (false);

	if (beta + gamma > 1.0)
		return (false);

	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;

	if (t < kEpsilon) 
		return (false);

	tmin = t;

	return (true);	
}

const BBox* MeshTriangle::get_bounding_box() const
{
	return &m_bbox;
}

void MeshTriangle::compute_normal(const bool reverse_normal)
{
	Vector3D v1 = m_mesh->vertices[m_index1] - m_mesh->vertices[m_index0];
	Vector3D v2 = m_mesh->vertices[m_index2] - m_mesh->vertices[m_index0];

	m_normal = v1 ^ v2;
	m_normal.normalize();

	if (reverse_normal)
		m_normal = -m_normal;
}

void MeshTriangle::set_mesh(Mesh* mesh, const int i1, const int i2, const int i3)
{
	m_mesh = mesh; m_index0 = i1; m_index1 = i2; m_index2 = i3;
	compute_normal(false);
	update_bbox();
}

Normal MeshTriangle::get_normal() const
{
	return m_normal;
}

void MeshTriangle::update_bbox()
{
	double delta = 0.0000001;

	Point3D p0 = m_mesh->vertices[m_index0];
	Point3D p1 = m_mesh->vertices[m_index1];
	Point3D p2 = m_mesh->vertices[m_index2];

	m_bbox.x0 = std::min(std::min(p0.x, p1.x), p2.x) - delta;
	m_bbox.y0 = std::min(std::min(p0.y, p1.y), p2.y) - delta;
	m_bbox.z0 = std::min(std::min(p0.z, p1.z), p2.z) - delta;
	m_bbox.x1 = std::max(std::max(p0.x, p1.x), p2.x) + delta;
	m_bbox.y1 = std::max(std::max(p0.y, p1.y), p2.y) + delta;
	m_bbox.z1 = std::max(std::max(p0.z, p1.z), p2.z) + delta;
}