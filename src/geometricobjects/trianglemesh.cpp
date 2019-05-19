#include <iostream>
#include "trianglemesh.h"
#include "utilities/ply.h"
#include "utilities/mesh.h"
#include "flatmeshtriangle.h"
#include "smoothmeshtriangle.h"

TriangleMesh::TriangleMesh(Mesh* mesh, bool reverse_normal)
	: Grid(), m_mesh(mesh), m_reverse_normal(reverse_normal)
{}

void TriangleMesh::ReadFlatTriangles(const char* ply_filepath)
{
	ReadPlyFile(ply_filepath, FLAT);
}

void TriangleMesh::ReadSmoothTriangles(const char* ply_filepath)
{
	ReadPlyFile(ply_filepath, SMOOTH);
	ComputeMeshNormals();
}

void TriangleMesh::ReadPlyFile(const char* ply_filepath, TriangleType type)
{
	// Vertex layout declaration
	PlyProperty vert_props[] = 
	{
		{"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex, x), 0, 0, 0, 0},
		{"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex, y), 0, 0, 0, 0},
		{"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex, z), 0, 0, 0, 0},
	};

	PlyProperty face_props[] = 
	{
		{"vertex_indices", PLY_INT, PLY_INT, offsetof(Face, verts),
			1, PLY_UCHAR, PLY_UCHAR, offsetof(Face, nverts)}
	};

	PlyFile* ply = nullptr;
	int nelem_types;
	char** elist;
	int file_type;
	float version;
	ply = ply_open_for_reading(const_cast<char*>(ply_filepath),
		&nelem_types, &elist, &file_type, &version);

	// Go through each type of element
	for (int i = 0; i < nelem_types; i++)
	{
		char* elem_name = elist[i];
		PlyProperty** plist;
		int nprops;
		int num_elems;
		plist = ply_get_element_description(ply,
			elem_name, &num_elems, &nprops);
		if (equal_strings("vertex", elem_name))
		{
			ReadPlyVertices(ply, elem_name, vert_props, num_elems);
		}
		else if (equal_strings("face", elem_name))
		{
			ReadPlyFaces(ply, elem_name, face_props, num_elems, type);
		}
	}

	ply_close(ply);
}

void TriangleMesh::ReadPlyVertices(PlyFile* ply, char* elem_name, 
									 PlyProperty* vert_props, int num_elems)
{
	ply_get_property(ply, elem_name, &vert_props[0]);
	ply_get_property(ply, elem_name, &vert_props[1]);
	ply_get_property(ply, elem_name, &vert_props[2]);

	m_mesh->num_verties = num_elems;
	m_mesh->vertices.reserve(num_elems);

	for (int i = 0; i < num_elems; i++)
	{
		Vertex* vertex = new Vertex;
		ply_get_element(ply, (void*) vertex);
		m_mesh->vertices.push_back(Point3D(
			vertex->x, vertex->y, vertex->z));
		delete vertex;
	}
}

void TriangleMesh::ReadPlyFaces(PlyFile* ply, char* elem_name, 
									PlyProperty* faces_props, int num_elems,
									TriangleType type)
{
	ply_get_property(ply, elem_name, &faces_props[0]);

	m_mesh->num_triangles = num_elems;
	// Faces will be saved in Compound::m_objects for now.
	m_objects.reserve(num_elems);

	std::vector<int> face_list;
	m_mesh->vertex_faces.reserve(m_mesh->num_verties);
	for (int i = 0; i < m_mesh->num_verties; i++)
		m_mesh->vertex_faces.push_back(face_list);

	for (int i = 0; i < num_elems; i++)
	{
		Face* face = new Face;
		ply_get_element(ply, (void*)face);

		if (type == FLAT)
		{
			FlatMeshTriangle* triangle = new FlatMeshTriangle(m_mesh,
				face->verts[0], face->verts[1], face->verts[2]);
			triangle->ComputeNormal(m_reverse_normal);
			m_objects.push_back(triangle);
		}
		else
		{
			SmoothMeshTriangle* triangle = new SmoothMeshTriangle(m_mesh,
				face->verts[0], face->verts[1], face->verts[2]);
			triangle->ComputeNormal(m_reverse_normal);
			m_objects.push_back(triangle);

			m_mesh->vertex_faces[face->verts[0]].push_back(i);
			m_mesh->vertex_faces[face->verts[1]].push_back(i);
			m_mesh->vertex_faces[face->verts[2]].push_back(i);
		}
	}

	if (type == FLAT)
		m_mesh->vertex_faces.clear();
}

void TriangleMesh::ComputeMeshNormals()
{
	m_mesh->normals.reserve(m_mesh->num_verties);

	// iterate the vertices
	for (int index = 0; index < m_mesh->num_verties; index++)
	{
		// iterate the faces that shares the vertex
		Normal normal;
		for (int j = 0; j < m_mesh->vertex_faces[index].size(); j++)
			normal += m_objects[m_mesh->vertex_faces[index][j]]->GetNormal();

		if (normal.x == 0.0 && normal.y == 0.0 && normal.z == 0.0)
			normal.y = 1.0f;
		else
			normal.normalize();

		m_mesh->normals.push_back(normal);
	}
}


