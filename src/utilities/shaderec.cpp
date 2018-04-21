#include "shaderec.h"

ShadeRec::ShadeRec(World& wr)
	:	hit_an_object(false),
		material_ptr(nullptr),
		hit_point(),
		local_hit_point(),
		normal(),
		ray(),
		depth(0),
		dir(),
		t(0.0),
		w(wr),
		color(RGBColor(0.0f))
{
}

ShadeRec::ShadeRec(const ShadeRec& sr)
	: 	hit_an_object(sr.hit_an_object),
		material_ptr(sr.material_ptr),
		hit_point(sr.hit_point),
		local_hit_point(sr.local_hit_point),
		normal(sr.normal),
		ray(sr.ray),
		depth(sr.depth),
		dir(sr.dir),
		t(sr.t),
		w(sr.w),
		color(sr.color)
{
}

ShadeRec& ShadeRec::operator=(const ShadeRec& rhs)
{
	if (this == &rhs)
		return *this;
	// not copy world
	this->hit_an_object = rhs.hit_an_object;
	this->hit_point = rhs.hit_point;
	this->normal = rhs.normal;
	this->color = rhs.color;
	this->local_hit_point = rhs.local_hit_point;
	this->material_ptr = rhs.material_ptr;
	this->ray = rhs.ray;
	this->depth = rhs.depth;
	this->dir = rhs.dir;
	this->t = rhs.t;

	return (*this);
}