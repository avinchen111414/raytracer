#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "utilities/point3d.h"
#include "utilities/point2d.h"
#include "utilities/normal.h"

class Mesh
{
public:
	Mesh();
	Mesh(const Mesh& other);
	~Mesh();
	Mesh& operator= (const Mesh& rhs);

	std::vector<Point3D> vertices;
	std::vector<Normal> normals;
	std::vector<Point2D> uv;
	std::vector<std::vector<int>> vertex_faces;	// similar to indices

	size_t num_verties;
	size_t num_triangles;
};

#endif