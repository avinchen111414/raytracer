#ifndef __TRIANGLEMESH_H__
#define __TRIANGLEMESH_H__

#include "grid.h"
#include "utilities/ply.h"

class Mesh;

enum TriangleType
{
	FLAT = 0,
	SMOOTH
};

class TriangleMesh: public Grid
{
public:
	typedef struct _Vertex
	{
		float x, y, z;	// position
	} Vertex;

	typedef struct _Face
	{
		unsigned char nverts; // number of vertex indices in the verts
		int* verts;	// vertex indices
	} Face;

public:
	TriangleMesh(Mesh* mesh, bool reverse_normal=false);
	void ReadFlatTriangles(const char* ply_filepath);
	void ReadSmoothTriangles(const char* ply_filepath);

protected:
	void ReadPlyFile(const char* ply_filepath, TriangleType type);
	void ComputeMeshNormals();
	void ReadPlyVertices(PlyFile* ply, char* elem_name,
		PlyProperty* vert_props, int num_elems);
	void ReadPlyFaces(PlyFile* ply, char* elem_name,
		PlyProperty* indices_props, int num_elems, TriangleType type);

private:
	Mesh* m_mesh;
	bool m_reverse_normal;
};

#endif