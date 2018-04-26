#include "world.h"
#include "sampler/jittered.h"
#include "sampler/hammersley.h"
#include "tracer/raycast.h"
#include "tracer/arealighting.h"
#include "lights/ambientoccluder.h"
#include "lights/arealight.h"
#include "camera/pinhole.h"
#include "materials/matte.h"
#include "materials/emissive.h"
#include "geometricobjects/plane.h"
#include "geometricobjects/rectangle.h"

void World::build_ao_scene()
{
	int num_samples = 256;

	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);

	Jittered* sampler_ptr = new Jittered(num_samples);
	AmbientOccluder* occluder = new AmbientOccluder;
	occluder->set_radiance(1.0f);
	occluder->set_color(1.0f);
	occluder->set_min_amount(0.0f);
	occluder->set_sampler(sampler_ptr);
	set_ambient_light(occluder);

	PinHole* camera = new PinHole;
	camera->eye = Point3D(25, 20, 45);
	camera->lookat = Point3D(0, 1, 0);
	camera->d = 5000;
	camera->compute_uvw();
	set_camera(camera);

	Matte* matte_ptr0 = new Matte;
	matte_ptr0->set_ka(0.75);
	matte_ptr0->set_kd(0);
	matte_ptr0->set_cd(RGBColor(1, 1, 0));

	Sphere* sphere0 = new Sphere(Point3D(0, 1, 0), 1);
	sphere0->set_material(matte_ptr0);
	add_object(sphere0);

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.75);
	matte_ptr1->set_kd(0);
	matte_ptr1->set_cd(RGBColor(1, 1, 1));

	Plane* plane = new Plane(Point3D(0), Normal(0, 1, 0));
	plane->set_material(matte_ptr1);
	add_object(plane);
}

void World::build_area_lights_scene()
{
	int num_samples = 1;

	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_samples(num_samples);

	//tracer_ptr = new RayCast(this);
	tracer_ptr = new AreaLighting(this);

	Jittered* sampler_ptr = new Jittered(num_samples);
	AmbientOccluder* occluder = new AmbientOccluder;
	occluder->set_radiance(0.3f);
	occluder->set_color(1.0f);
	occluder->set_min_amount(0.0f);
	occluder->set_sampler(sampler_ptr);
	set_ambient_light(occluder);

	Emissive* emmisvie_ptr = new Emissive;
	emmisvie_ptr->set_ls(40.0f);
	emmisvie_ptr->set_ce(RGBColor(1.0f));

	Jittered* rectangle_sampler_ptr = new Jittered(num_samples);
	raytracer::Rectangle* rectangle_ptr = new raytracer::Rectangle(Point3D(-2.0f, 0.0f, -2.0f),
		Vector3D(0.0f, 3.0f, 0.0f), Vector3D(-1.7f, 0.0f, 1.7f));
	rectangle_ptr->set_material(emmisvie_ptr);
	rectangle_ptr->set_sampler(rectangle_sampler_ptr);
	rectangle_ptr->enable_shadow(false);
	add_object(rectangle_ptr);

	AreaLight* area_light_ptr = new AreaLight;
	area_light_ptr->set_object(rectangle_ptr);
	area_light_ptr->set_cast_shadow(true);
	add_light(area_light_ptr);

	PinHole* camera = new PinHole;
	camera->eye = Point3D(50, 40, 90);
	camera->lookat = Point3D(0, 1, 0);
	camera->d = 5000;
	camera->compute_uvw();
	set_camera(camera);

	Matte* matte_ptr0 = new Matte;
	matte_ptr0->set_ka(0.75);
	matte_ptr0->set_kd(0);
	matte_ptr0->set_cd(RGBColor(1, 1, 0));

	Sphere* sphere0 = new Sphere(Point3D(0, 1, 0), 1);
	sphere0->set_material(matte_ptr0);
	add_object(sphere0);

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.75);
	matte_ptr1->set_kd(0);
	matte_ptr1->set_cd(RGBColor(1, 1, 1));

	Plane* plane = new Plane(Point3D(0), Normal(0, 1, 0));
	plane->set_material(matte_ptr1);
	add_object(plane);
}
