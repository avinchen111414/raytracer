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
	copy_objects(object.m_objects);
}

Compound& Compound::operator= (const Compound& rhs)
{
	if (&rhs == this)
		return *this;

	GeometricObject::operator=(rhs);
	copy_objects(rhs.m_objects);
	return *this;
}

Compound::~Compound()
{
	delete_objects();
}

GeometricObject* Compound::clone() const
{
	return new Compound(*this);
}

bool Compound::hit(const Ray& ray, double& tmin, ShadeRec& s) const
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
		if (object->hit(ray, t, s) && t < tmin)
		{
			hit = true;
			tmin = t;
			tmin_normal = s.normal;
			tmin_local_hit_point = s.local_hit_point;
			tmin_material = object->get_material();
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

void Compound::set_material(Material* m)
{
	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		m_objects[idx]->set_material(m);
	}
}

void Compound::add_object(GeometricObject* object)
{
	m_objects.push_back(object);
	update_bbox();
}

size_t Compound::get_object_num() const
{
	return m_objects.size();
}

void Compound::delete_objects()
{
	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		delete m_objects[idx];
		m_objects[idx] = nullptr;
	}

	m_objects.clear();
	update_bbox();
}

void Compound::copy_objects(const std::vector<GeometricObject *>& rhs_objects)
{
	delete_objects();
	for (size_t idx = 0; idx < rhs_objects.size(); idx++)
	{
		m_objects.push_back(rhs_objects[idx]->clone());
	}
	update_bbox();
}

bool Compound::shadow_hit(const Ray& ray, float& tmin) const
{
	if (!m_shadow)
		return false;

	bool hit = false;
	float t;
	tmin = INT_MAX;

	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		const GeometricObject* object = m_objects[idx];
		if (object->shadow_hit(ray, t) && t < tmin)
		{
			hit = true;
			tmin = t;
		}
	}

	return hit;
}

const BBox* Compound::get_bounding_box() const
{
	return &m_bbox;
}

void Compound::update_bbox()
{
	m_bbox = BBox();
	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		const BBox* obj_bbox = m_objects[idx]->get_bounding_box();
		if (!obj_bbox)
			continue;
		m_bbox.Merge(*obj_bbox);
	}
}