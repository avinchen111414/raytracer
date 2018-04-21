#include "rectangle.h"
#include "sampler/sampler.h"

const double Rectangle::kEpsilon = 0.001;

Rectangle::Rectangle():
	GeometricObject(),
	p0(Point3D(-1, 0, -1)),
	a(Vector3D(0.0f, 0.0f, 2.0f)),
	b(Vector3D(2.0f, 0.0f, 0.0f)),
	a_len_squared(4.0f),
	b_len_squared(4.0f),
	normal(Vector3D(0, 1, 0)),
	area(4),
	inv_area(0.25f),
	sampler_ptr(nullptr)
{

}