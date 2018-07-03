#include "flatmeshtriangle.h"
#include "utilities/ray.h"
#include "utilities/utility.h"
#include "utilities/shaderec.h"
#include "utilities/normal.h"
#include "utilities/mesh.h"

FlatMeshTriangle::FlatMeshTriangle(const FlatMeshTriangle& other):
	MeshTriangle(other)
{
}

FlatMeshTriangle::FlatMeshTriangle(Mesh* _mesh_ptr, const int i1, const int i2, const int i3):
	MeshTriangle(_mesh_ptr, i1, i2, i3)
{
}

GeometricObject* FlatMeshTriangle::clone() const
{
	return new FlatMeshTriangle(*this);
}

bool FlatMeshTriangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const
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

	tmin 				= t;
	sr.normal 			= m_normal;  	
	sr.local_hit_point 	= ray.o + t * ray.d;
	sr.material_ptr = get_material();

	return (true);
}

bool FlatMeshTriangle::shadow_hit(const Ray& ray, float& tmin) const
{
	return MeshTriangle::shadow_hit(ray, tmin);
}