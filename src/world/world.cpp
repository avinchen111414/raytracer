#include <cmath>
#include "utilities/ray.h"
#include "utilities/utility.h"
#include "utilities/shaderec.h"
#include "utilities/point2d.h"

#include "geometricobjects/plane.h"
#include "geometricobjects/rectangle.h"

#include "world/world.h"

#include "tracer/tracer.h"
#include "tracer/singlesphere.h"
#include "tracer/multipleobjects.h"
#include "tracer/raycast.h"

#include "app/wxraytracer.h"

#include "sampler/jittered.h"
#include "sampler/hammersley.h"
#include "camera/pinhole.h"
#include "lights/direction.h"
#include "lights/ambient.h"
#include "lights/ambientoccluder.h"
#include "lights/point.h"

#include "materials/matte.h"
#include "materials/phong.h"
#include "materials/emissive.h"


World::World(void)
	:  	background_color(),
		tracer_ptr(NULL),
		camera_ptr(nullptr),
		ambient_ptr(new Ambient)
{}

World::~World(void) {	
	
	if(tracer_ptr) {
		delete tracer_ptr;
		tracer_ptr = NULL;
	}				
}

void 												
World::render_scene(void) const {

	RGBColor	pixel_color;	 	
	Ray			ray;					
	int 		hres 	= vp.hres;
	int 		vres 	= vp.vres;
	float		s		= vp.s;
	float		zw		= 100.0;				// hardwired in
	// sample param
	Point2D pp;
	Point2D sp;

	ray.d = Vector3D(0, 0, -1);
	
	for (int r = 0; r < vres; r++)			// up
		for (int c = 0; c <= hres; c++)		// across
		{	
			pixel_color = RGBColor(0.0f);
			for (int i = 0; i != vp.num_samples; i++)
			{
				// filter sampling
				sp = vp.sampler_ptr->sample_unit_square();
				pp.x = (c - vp.hres * 0.5f + sp.x);
				pp.y = (r - vp.vres * 0.5f + sp.y);
				ray.o = Point3D(pp.x, pp.y, zw);
				pixel_color += tracer_ptr->trace_ray(ray);
			}
			pixel_color /= vp.num_samples;
			display_pixel(r, c, pixel_color);
		}	
} 

void World::render_perspective() const
{
	RGBColor pixel_color;
	Ray ray;
	Point2D sp;
	Point2D pp;

	ray.o = Point3D(0.0f, 0.0f, eye);
	for (int r = 0; r != vp.vres; r++)
		for (int c = 0; c != vp.hres; c++)
		{
			pixel_color = RGBColor(0.0f);
			for (int i = 0; i != vp.num_samples; i++)
			{
				// filter sampling
				sp = vp.sampler_ptr->sample_unit_square();
				pp.x = vp.s * (c - 0.5f * (vp.hres - 1) + sp.x);
				pp.y = vp.s * (r - 0.5f * (vp.vres - 1) + sp.y);
				ray.d = Point3D(pp.x, pp.y, -d);
				ray.d.normalize();
				pixel_color += tracer_ptr->trace_ray(ray);
			}
			pixel_color /= vp.num_samples;
			display_pixel(r, c, pixel_color);
		}
}

void
World::display_pixel(const int row, const int column, const RGBColor& raw_color) const {
	RGBColor mapped_color = raw_color;
	if (vp.show_out_of_gamut)
		mapped_color = clamp_to_color(raw_color);
	else
		mapped_color = max_to_one(raw_color);
	
	if (vp.gamma != 1.0)
		mapped_color = mapped_color.powc(vp.inv_gamma);
	
   //have to start from max y coordinate to convert to screen coordinates
   int x = column;
   int y = vp.vres - row - 1;

   paintArea->setPixel(x, y, (int)(mapped_color.r * 255),
                             (int)(mapped_color.g * 255),
                             (int)(mapped_color.b * 255));
}

RGBColor
World::max_to_one(const RGBColor& c) const  {
	float max_value = std::max(c.r, std::max(c.g, c.b));
	
	if (max_value > 1.0)
		return (c / max_value);
	else
		return (c);
}


// ------------------------------------------------------------------ clamp_to_color
// Set color to red if any component is greater than one

RGBColor
World::clamp_to_color(const RGBColor& raw_color) const {
	RGBColor c(raw_color);
	
	if (raw_color.r > 1.0 || raw_color.g > 1.0 || raw_color.b > 1.0) {
		c.r = 1.0; c.g = 0.0; c.b = 0.0;
	}
		
	return (c);
}

void World::build()
{
	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_pixel_size(1);
	vp.set_gamma(1.0);

	const int num_samples = 24;
	Sampler* sampler_ptr = new Hammersley(num_samples, 16);
	vp.set_sampler(sampler_ptr);

	Ambient* ambient_ptr = new Ambient;
	ambient_ptr->scale_radiance(0.1f);
	set_ambient_light(ambient_ptr);

	PinHole* camera_ptr = new PinHole;
	camera_ptr->eye = Point3D(300, 300, 600);
	camera_ptr->lookat = Point3D(0, 0, 0);
	camera_ptr->d = 400;
	camera_ptr->compute_uvw();
	set_camera(camera_ptr);
	
	Point* point_light = new Point;
	point_light->set_location(Point3D(70, 100, 100));
	point_light->scale_radiance(0.5);
	point_light->set_cast_shadow(true);
	add_light(point_light);

	background_color = RGBColor(0.0f);
	tracer_ptr = new RayCast(this);

	raytracer::Rectangle* rect = new raytracer::Rectangle(Point3D(0.0f, 0.0f, -300.0f),
		Vector3D(100.0f, 0.0f, 0.0f), Vector3D(0.0f, 100.0f, 0.0f));
	Emissive* emissive_ptr = new Emissive;
	rect->set_material(emissive_ptr);
	add_object(rect);

	Matte* matte_ptr = new Matte;
	matte_ptr->set_ka(1.0f);
	matte_ptr->set_kd(0.75f);
	matte_ptr->set_cd(RGBColor(0.5, 0.5, 0.5));
	matte_ptr->enable_recv_shadow(true);

	Phong* phong_ptr = new Phong;
	phong_ptr->set_ka(0.5f);
	phong_ptr->set_kd(0.75f);
	phong_ptr->set_cd(RGBColor(1.0f, 1.0f, 0.0f));
	phong_ptr->set_ks(0.15f);
	phong_ptr->set_cs(RGBColor(1.0f, 1.0f, 0.0f));
	phong_ptr->set_exp(1);
	phong_ptr->enable_recv_shadow(true);

	Sphere* sphere_ptr = nullptr;
	sphere_ptr = new Sphere(Point3D(0, 60, 0), 60);
	sphere_ptr->set_material(phong_ptr);
	add_object(sphere_ptr);

	Plane* plane_ptr = new Plane(Point3D(0.0f, 0.0f, 0.0f), Normal(0, 1, 0));
	plane_ptr->set_material(matte_ptr);
	add_object(plane_ptr);
}

ShadeRec World::hit_bare_bones_objects(const Ray& ray)
{
	ShadeRec sr(*this);
	double t;
	double tmin = INT_MAX;
	size_t num_objs = objects.size();

	for (size_t i = 0; i != num_objs; i++)
	{
		ShadeRec sr_stack(sr);	
		if (objects[i]->hit(ray, t, sr) && (t < tmin))
		{
			sr.hit_an_object = true;
			tmin = t;
			sr.color = objects[i]->get_color();
		}
		else
		{
			sr = sr_stack;
		}
	}
	return sr;
}

ShadeRec World::hit_object(const Ray& ray)
{
	ShadeRec sr(*this);
	double t;
	double tmin = INT_MAX;
	Normal normal;
	Point3D local_hit_point;
	size_t num_objects = objects.size();

	for (size_t i = 0; i != num_objects; i++)
	{
		if (objects[i]->hit(ray, t, sr) && (t < tmin))
		{
			sr.hit_an_object = true;
			tmin = t;
			sr.material_ptr = objects[i]->get_material();
			sr.hit_point = ray.o + t * ray.d;
			normal = sr.normal;
			local_hit_point = sr.local_hit_point;
		}
	}

	if (sr.hit_an_object)
	{
		sr.t = tmin;
		sr.normal = normal;
		sr.local_hit_point = local_hit_point;
	}

	return sr;
}