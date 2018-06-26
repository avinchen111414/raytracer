#include "mesh.h"

Mesh::Mesh()
	: num_verties(0), num_triangles(0)
{

}

Mesh::Mesh(const Mesh& other)
	: vertices(other.vertices),
	normals(other.normals),
	uv(other.uv),
	vertex_faces(other.vertex_faces),
	num_verties(other.num_verties),
	num_triangles(other.num_triangles)
{

}

Mesh::~Mesh()
{

}

Mesh& Mesh::operator= (const Mesh& rhs)
{
	if (&rhs == this)
		return *this;

	vertices = rhs.vertices;
	normals = rhs.normals;
	uv = rhs.uv;
	vertex_faces = rhs.vertex_faces;
	num_verties = rhs.num_verties;
	num_triangles = rhs.num_triangles;
	return *this;
}