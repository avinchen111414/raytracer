#include "compound.h"
#include "utilities/shaderec.h"
#include "materials/material.h"

Compound::Compound()
	: GeometricObject()
{

}

Compound::Compound(const Compound& object)
	: GeometricObject(object)
{
	CopyObjects(object.m_objects);
}

Compound& Compound::operator= (const Compound& rhs)
{
	if (&rhs == this)
		return *this;

	GeometricObject::operator=(rhs);
	CopyObjects(rhs.m_objects);
	return *this;
}

Compound::~Compound()
{
	DeleteObjects();
}

GeometricObject* Compound::Clone() const
{
	return new Compound(*this);
}

bool Compound::Hit(const Ray& ray, double& tmin, ShadeRec& s) const
{
	bool hit = false;
	double t;
	tmin = INT_MAX;
	Normal tmin_normal;
	Point3D tmin_local_hit_point;
	Material* tmin_material = NULL;

	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		const GeometricObject* object = m_objects[idx];
		if (object->Hit(ray, t, s) && t < tmin)
		{
			hit = true;
			tmin = t;
			tmin_normal = s.normal;
			tmin_local_hit_point = s.local_hit_point;
			tmin_material = object->GetMaterial();
		}
	}

	if (hit)
	{
		s.t = tmin;
		s.normal = tmin_normal;
		s.local_hit_point = tmin_local_hit_point;
		const_cast<Material*>(m_material) = tmin_material;
	}

	return hit;
}

void Compound::SetMaterial(Material* m)
{
	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		m_objects[idx]->SetMaterial(m);
	}
}

void Compound::AddObject(GeometricObject* object)
{
	m_objects.push_back(object);
	UpdateBbox();
}

size_t Compound::GetObjectNum() const
{
	return m_objects.size();
}

void Compound::DeleteObjects()
{
	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		delete m_objects[idx];
		m_objects[idx] = nullptr;
	}

	m_objects.clear();
	UpdateBbox();
}

void Compound::CopyObjects(const std::vector<GeometricObject *>& rhs_objects)
{
	DeleteObjects();
	for (size_t idx = 0; idx < rhs_objects.size(); idx++)
	{
		m_objects.push_back(rhs_objects[idx]->Clone());
	}
	UpdateBbox();
}

bool Compound::ShadowHit(const Ray& ray, float& tmin) const
{
	if (!m_shadow)
		return false;

	bool hit = false;
	float t;
	tmin = INT_MAX;

	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		const GeometricObject* object = m_objects[idx];
		if (object->ShadowHit(ray, t) && t < tmin)
		{
			hit = true;
			tmin = t;
		}
	}

	return hit;
}

const BBox* Compound::GetBoundingBox() const
{
	return &m_bbox;
}

void Compound::UpdateBbox()
{
	m_bbox = BBox();
	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		const BBox* obj_bbox = m_objects[idx]->GetBoundingBox();
		if (!obj_bbox)
			continue;
		m_bbox.Merge(*obj_bbox);
	}
}