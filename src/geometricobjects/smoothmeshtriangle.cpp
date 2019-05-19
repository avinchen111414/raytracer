#include "smoothmeshtriangle.h"
#include "utilities/utility.h"
#include "utilities/mesh.h"
#include "utilities/ray.h"
#include "utilities/shaderec.h"

SmoothMeshTriangle::SmoothMeshTriangle(const SmoothMeshTriangle& other)
	: FlatMeshTriangle(other), m_n0(other.m_n0), m_n1(other.m_n1), m_n2(other.m_n2)
{

}

SmoothMeshTriangle::SmoothMeshTriangle(Mesh* _mesh_ptr, const int i1, const int i2, const int i3)
	: FlatMeshTriangle(_mesh_ptr, i1, i2, i3), m_n0(Normal(0, 1, 0)),
	m_n1(Normal(0, 1, 0)), m_n2(Normal(0, 1, 0))
{

}

bool SmoothMeshTriangle::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
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
	sr.normal 			= InterpolateNormal(beta, gamma);  	
	sr.local_hit_point 	= ray.o + t * ray.d;

	return (true);
}

Normal SmoothMeshTriangle::InterpolateNormal(const float beta, const float gamma) const
{
	Normal normal((1 - beta - gamma) * m_mesh->normals[m_index0] 
	+ beta * m_mesh->normals[m_index1] 
	+ gamma * m_mesh->normals[m_index2]);
	normal.normalize();

	return(normal);
}