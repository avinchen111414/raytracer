#include <algorithm>
#include "box.h"
#include "geometricobject.h"
#include "utilities/ray.h"
#include "utilities/shaderec.h"

const float Box::kEpsilon = 0.001f;

Box::Box(float _x0, float _y0, float _z0,
		 float _x1, float _y1, float _z1)
		 : GeometricObject(), 
		 m_x0(_x0), m_y0(_y0), m_z0(_z0), m_x1(_x1), m_y1(_y1), m_z1(_z1),
		 m_bbox(_x0, _y0, _z0, _x1 , _y1, _z1)
{}

Box::Box(const Point3D& p0, const Point3D& p1)
	: GeometricObject(),
	m_x0(p0.x), m_y0(p0.y), m_z0(p0.z),
	m_x1(p1.x), m_y1(p1.y), m_z1(p1.z),
	m_bbox(p0, p1)
{}

Box::Box(const Box& other)
	: GeometricObject(other),
	m_x0(other.m_x0), m_y0(other.m_y0), m_z0(other.m_z0),
	m_x1(other.m_x1), m_y1(other.m_y1), m_z1(other.m_z1),
	m_bbox(other.m_bbox)
{}

Box& Box::operator= (const Box& rhs)
{
	if (this == &rhs)
		return *this;

	GeometricObject::operator=(rhs);
	m_x0 = rhs.m_x0; m_y0 = rhs.m_y0; m_z0 = rhs.m_z0;
	m_x1 = rhs.m_x1; m_y1 = rhs.m_y1; m_z1 = rhs.m_z1;
	m_bbox = rhs.m_bbox;

	return *this;
}

Box::~Box()
{}

GeometricObject* Box::Clone() const
{
	return new Box(*this);
}

bool Box::Hit(const Ray& ray, double& t, ShadeRec& s) const 
{
	double ox = ray.o.x; double dx = ray.d.x;
	double tx_min, tx_max;
	double a = 1.0 / dx;
	if (a >= 0)
	{
		tx_min = (m_x0 - ox) * a;
		tx_max = (m_x1 - ox) * a;
	}
	else
	{
		tx_min = (m_x1 - ox) * a;
		tx_max = (m_x0 - ox) * a;
	}

	double oy = ray.o.y; double dy = ray.d.y;
	double ty_min, ty_max;
	double b = 1.0 / dy;
	if (b >= 0)
	{
		ty_min = (m_y0 - oy) * b;
		ty_max = (m_y1 - oy) * b;
	}
	else
	{
		ty_min = (m_y1 - oy) * b;
		ty_max = (m_y0 - oy) * b;
	}

	double oz = ray.o.z; double dz = ray.d.z;
	double tz_min, tz_max;
	double c = 1.0 / dz;
	if (c >= 0)
	{
		tz_min = (m_z0 - oz) * c;
		tz_max = (m_z1 - oz) * c;
	}
	else
	{
		tz_min = (m_z1 - oz) * c;
		tz_max = (m_z0 - oz) * c;
	}

	// find the largest entering t-value
	int face_in;
	double t0;
	if (tx_min > ty_min)
	{
		t0 = tx_min;
		face_in = (a >= 0.0) ? 0 : 3;
	}
	else
	{
		t0 = ty_min;
		face_in = (b >= 0.0) ? 1 : 4;
	}
	if (tz_min > t0)
	{
		t0 = tz_min;
		face_in = (c >= 0.0) ? 2 : 5;
	}

	// find the smallest exiting t-value
	int face_out;
	double t1;
	if (tx_max < ty_max)
	{
		t1 = tx_max;
		face_out = (a >= 0) ? 3 : 0;
	}
	else
	{
		t1 = ty_max;
		face_out = (b >= 0) ? 4 : 1;
	}
	if (tz_max < t1)
	{
		t1 = tz_max;
		face_out = (c >= 0) ? 5 : 2;
	}

	if (t0 < t1 && t1 > kEpsilon)
	{
		// ray hits outside surface
		if (t0 > kEpsilon)
		{
			t = t0;
			s.normal = GetNormal(face_in);
		}
		else // ray hits inside surface
		{
			t = t1;
			s.normal = GetNormal(face_out);
		}

		s.local_hit_point = ray.o + t * ray.d;
		return true;
	}
	else
		return false;
}

bool Box::ShadowHit(const Ray& ray, float& tmin) const
{
	double ox = ray.o.x; double dx = ray.d.x;
	double tx_min, tx_max;
	double a = 1.0 / dx;
	if (a >= 0)
	{
		tx_min = (m_x0 - ox) * a;
		tx_max = (m_x1 - ox) * a;
	}
	else
	{
		tx_min = (m_x1 - ox) * a;
		tx_max = (m_x0 - ox) * a;
	}

	double oy = ray.o.y; double dy = ray.d.y;
	double ty_min, ty_max;
	double b = 1.0 / dy;
	if (b >= 0)
	{
		ty_min = (m_y0 - oy) * b;
		ty_max = (m_y1 - oy) * b;
	}
	else
	{
		ty_min = (m_y1 - oy) * b;
		ty_max = (m_y0 - oy) * b;
	}

	double oz = ray.o.z; double dz = ray.d.z;
	double tz_min, tz_max;
	double c = 1.0 / dz;
	if (c >= 0)
	{
		tz_min = (m_z0 - oz) * c;
		tz_max = (m_z1 - oz) * c;
	}
	else
	{
		tz_min = (m_z1 - oz) * c;
		tz_max = (m_z0 - oz) * c;
	}

	// find the largest entering t-value
	double t0;
	if (tx_min > ty_min)
	{
		t0 = tx_min;
	}
	else
	{
		t0 = ty_min;
	}
	if (tz_min > t0)
	{
		t0 = tz_min;
	}

	// find the smallest exiting t-value
	double t1;
	if (tx_max < ty_max)
	{
		t1 = tx_max;
	}
	else
	{
		t1 = ty_max;
	}
	if (tz_max < t1)
	{
		t1 = tz_max;
	}

	if (t0 < t1 && t1 > kEpsilon)
	{
		// ray hits outside surface
		if (t0 > kEpsilon)
		{
			tmin = t0;
		}
		else // ray hits inside surface
		{
			tmin = t1;
		}
		return true;
	}
	else
		return false;
}

Normal Box::GetNormal(int face_hit) const
{
	switch (face_hit)
	{
	case 0:
		return Normal(-1.0f, 0.0f, 0.0f); // -x face
	case 1:
		return Normal(0.0f, -1.0f, 0.0f); // -y face
	case 2:
		return Normal(0.0f, 0.0f, -1.0f); // -z face
	case 3:
		return Normal(1.0f, 0.0f, 0.0f); // +x face
	case 4:
		return Normal(0.0f, 1.0f, 0.0f); // +y face
	case 5:
		return Normal(0.0f, 0.0f, 1.0f); // +z face
	}

	// impossible reach here
	return Normal(0.0f, 0.0f, 0.0f);
}

const BBox* Box::GetBoundingBox() const
{
	return &m_bbox;
}
