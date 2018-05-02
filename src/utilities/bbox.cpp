#include <algorithm>
#include "bbox.h"
#include "point3d.h"
#include "ray.h"
#include "utility.h"

const double BBox::kEpsilon = 0.001;

BBox::BBox(double _x0, double _y0, double _z0,
		   double _x1, double _y1, double _z1)
		   : x0(_x0), y0(_y0), z0(_z0),
		   x1(_x1), y1(_y1), z1(_z1)
{}

BBox::BBox(const Point3D& p0, const Point3D& p1)
	: x0(p0.x), y0(p0.y), z0(p0.z),
	x1(p1.x), y1(p1.y), z1(p1.z)
{}

BBox::BBox(const BBox& other)
	: x0(other.x0), y0(other.y0), z0(other.z0),
	x1(other.x1), y1(other.y1), z1(other.z1)
{}

BBox& BBox::operator= (const BBox& rhs)
{
	if (this == &rhs)
		return *this;
	x0 = rhs.x0; y0 = rhs.y0; z0 = rhs.z0;
	x1 = rhs.x1; y1 = rhs.y1; z1 = rhs.z1;
	return *this;
}

BBox::~BBox()
{}

bool BBox::hit(const Ray& ray) const
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
	t0 = std::max(tx_min, ty_min);
	t0 = std::max(t0, tz_min);

	// find the smallest exiting t-value
	double t1;
	t1 = std::min(tx_max, ty_max);
	t1 = std::min(t1, tz_max);

	return (t0 < t1 && t1 > kEpsilon);
}