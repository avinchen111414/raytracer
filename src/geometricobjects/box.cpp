#include <algorithm>
#include "box.h"
#include "geometricobject.h"
#include "utilities/ray.h"
#include "utilities/shaderec.h"

const float Box::kEpsilon = 0.001f;

Box::Box(float _x0, float _y0, float _z0,
		 float _x1, float _y1, float _z1)
		 : GeometricObject(), 
		 x0(_x0), y0(_y0), z0(_z0), x1(_x1), y1(_y1), z1(_z1)
{}

Box::Box(const Point3D& p0, const Point3D& p1)
	: GeometricObject(),
	x0(p0.x), y0(p0.y), z0(p0.z),
	x1(p1.x), y1(p1.y), z1(p1.z)
{}

Box::Box(const Box& other)
	: GeometricObject(other),
	x0(other.x0), y0(other.y0), z0(other.z0),
	x1(other.x1), y1(other.y1), z1(other.z1)
{}

Box& Box::operator= (const Box& rhs)
{
	if (this == &rhs)
		return *this;

	GeometricObject::operator=(rhs);
	x0 = rhs.x0; y0 = rhs.y0; z0 = rhs.z0;
	x1 = rhs.x1; y1 = rhs.y1; z1 = rhs.z1;

	return *this;
}

Box::~Box()
{}

GeometricObject* Box::clone() const
{
	return new Box(*this);
}

bool Box::hit(const Ray& ray, double& t, ShadeRec& s) const 
{
	double ox = ray.o.x; double dx = ray.d.x;
	double tx_min, tx_max;
	double a = 1.0 / dx;
	if (a >= 0)
	{
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else
	{
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double oy = ray.o.y; double dy = ray.d.y;
	double ty_min, ty_max;
	double b = 1.0 / dy;
	if (b >= 0)
	{
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else
	{
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double oz = ray.o.z; double dz = ray.d.z;
	double tz_min, tz_max;
	double c = 1.0 / dz;
	if (c >= 0)
	{
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else
	{
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
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
			s.normal = get_normal(face_in);
		}
		else // ray hits inside surface
		{
			t = t1;
			s.normal = get_normal(face_out);
		}

		s.local_hit_point = ray.o + t * ray.d;
		return true;
	}
	else
		return false;
}

bool Box::shadow_hit(const Ray& ray, float& tmin) const
{
	double ox = ray.o.x; double dx = ray.d.x;
	double tx_min, tx_max;
	double a = 1.0 / dx;
	if (a >= 0)
	{
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else
	{
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double oy = ray.o.y; double dy = ray.d.y;
	double ty_min, ty_max;
	double b = 1.0 / dy;
	if (b >= 0)
	{
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else
	{
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double oz = ray.o.z; double dz = ray.d.z;
	double tz_min, tz_max;
	double c = 1.0 / dz;
	if (c >= 0)
	{
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else
	{
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
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

Normal Box::get_normal(int face_hit) const
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
