#include "world.h"
#include "sampler/jittered.h"
#include "sampler/hammersley.h"
#include "tracer/raycast.h"
#include "tracer/arealighting.h"
#include "tracer/globaltracer.h"
#include "lights/ambient.h"
#include "lights/ambientoccluder.h"
#include "lights/arealight.h"
#include "lights/environmentlight.h"
#include "lights/point.h"
#include "lights/direction.h"
#include "camera/pinhole.h"
#include "materials/matte.h"
#include "materials/emissive.h"
#include "materials/phong.h"
#include "materials/reflective.h"
#include "materials/glossyreflective.h"
#include "geometricobjects/plane.h"
#include "geometricobjects/rectangle.h"
#include "geometricobjects/box.h"
#include "geometricobjects/triangle.h"
#include "geometricobjects/instance.h"
#include "geometricobjects/grid.h"
#include "geometricobjects/trianglemesh.h"
#include "utilities/mesh.h"
#include "utilities/utility.h"

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
	int num_samples = 16;

	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_samples(num_samples);

	tracer_ptr = new AreaLighting(this);

	Jittered* sampler_ptr = new Jittered(num_samples);
	AmbientOccluder* occluder = new AmbientOccluder;
	occluder->set_radiance(0.3f);
	occluder->set_color(1.0f);
	occluder->set_min_amount(0.0f);
	occluder->set_sampler(sampler_ptr);
	set_ambient_light(occluder);

	Emissive* emmisvie_ptr = new Emissive;
	emmisvie_ptr->set_ls(5.0f);
	emmisvie_ptr->set_ce(RGBColor(1.0f));

	Jittered* rectangle_sampler_ptr = new Jittered(num_samples);
	raytracer::Rectangle* rectangle_ptr = new raytracer::Rectangle(Point3D(-2.0f, 1.0f, -2.0f),
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
	matte_ptr0->set_ka(0.25);
	matte_ptr0->set_kd(0.75);
	matte_ptr0->set_cd(RGBColor(1, 1, 0));

	Sphere* sphere0 = new Sphere(Point3D(0, 1, 0), 1);
	sphere0->set_material(matte_ptr0);
	add_object(sphere0);
	
	
	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.25);
	matte_ptr1->set_kd(0.75);
	matte_ptr1->set_cd(RGBColor(1, 1, 1));

	Plane* plane = new Plane(Point3D(0), Normal(0, 1, 0));
	plane->set_material(matte_ptr1);
	add_object(plane);

	Box* box = new Box(Point3D(1, 0, 1), Point3D(2, 1, 2));
	box->set_material(matte_ptr1);
	add_object(box);
}

void World::build_env_light_scene()
{
	build_area_lights_scene();

	Emissive* env_emissive_mtl = new Emissive;
	env_emissive_mtl->set_ce(RGBColor(0.8f, 0.3f, 0.1f));
	env_emissive_mtl->set_ls(1.0f);

	EnvironmentLight* env_light = new EnvironmentLight;
	env_light->set_sampler(new Jittered(256));
	env_light->set_material(env_emissive_mtl);
	add_light(env_light);
}

void World::build_instance_test_scene()
{
	int num_samples = 16;

	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_samples(num_samples);

	tracer_ptr = new AreaLighting(this);

	PinHole* camera = new PinHole;
	camera->eye = Point3D(100, 0, 100);
	camera->lookat = Point3D(0, 0, 0);
	camera->d = 8000;
	camera->compute_uvw();
	set_camera(camera);

	Point* point_light = new Point();
	point_light->set_location(Point3D(50.0, 50.0, 1.0));
	point_light->scale_radiance(3.0f);
	add_light(point_light);

	Phong* phong = new Phong;
	phong->set_cd(0.75);
	phong->set_ka(0.25);
	phong->set_kd(0.8);
	phong->set_ks(0.15);
	phong->set_exp(50);

	Instance* ellipsoid = new Instance(new Sphere);
	ellipsoid->set_material(phong);
	ellipsoid->scale(2, 3, 1);
	ellipsoid->rotate_x(-45);
	ellipsoid->translate(0, 1, 0);
	add_object(ellipsoid);
}

void World::build_grid_test_scene()
{
	int num_samples = 16;

	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_samples(num_samples);

	tracer_ptr = new AreaLighting(this);

	PinHole* camera = new PinHole;
	camera->eye = Point3D(0, 0, 1000);
	camera->lookat = Point3D(0, 0, 0);
	camera->d = 8000;
	camera->compute_uvw();
	set_camera(camera);

	Direction* dir = new Direction;
	dir->set_direction(Vector3D(-1.0f, -1.0f, -1.0f));
	dir->set_color(1.0f);
	dir->scale_radiance(1.0f);
	add_light(dir);

	Phong* phong = new Phong;
	phong->set_cd(0.75);
	phong->set_ka(0.25);
	phong->set_kd(0.8);
	phong->set_ks(0.15);
	phong->set_exp(50);

	Grid* grid = new Grid;

	for (int i = -50; i < 50; i++)
	{
		for (int j = -50; j < 50; j++)
		{
			Point3D center(i, j, 0);
			Sphere* sphere = new Sphere(center, 0.5f);
			sphere->set_material(phong);
			grid->add_object(sphere);
		}
	}
	
	grid->setup_cells();
	add_object(grid);
}

void World::build_triangle_mesh_test_scene()
{
	int num_samples = 16;

	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_samples(num_samples);

	tracer_ptr = new AreaLighting(this);

	PinHole* pinhole_ptr = new PinHole;
	pinhole_ptr->eye = Point3D(100, 50, 90);
	pinhole_ptr->lookat = Point3D(0, -0.5, 0);
	pinhole_ptr->d = 16000;  	
	pinhole_ptr->compute_uvw();     
	set_camera(pinhole_ptr);

	Jittered* sampler_ptr = new Jittered(num_samples);
	AmbientOccluder* occluder = new AmbientOccluder;
	occluder->set_radiance(1.0f);
	occluder->set_color(1.0f);
	occluder->set_min_amount(0.5f);
	occluder->set_sampler(sampler_ptr);
	set_ambient_light(occluder);

	Direction* directional_ptr = new Direction;
	directional_ptr->set_direction(Vector3D(-0.75, -1, 0.15));     
	directional_ptr->scale_radiance(4.5);  
	directional_ptr->set_cast_shadow(true);
	add_light(directional_ptr);

	Matte* matte_ptr1 = new Matte;			
	matte_ptr1->set_ka(0.1); 
	matte_ptr1->set_kd(0.75);   
	matte_ptr1->set_cd(RGBColor(0.1, 0.5, 1.0));
	matte_ptr1->enable_recv_shadow(true);

	Phong* phong = new Phong;
	phong->set_cd(0.75);
	phong->set_ka(0.3);
	phong->set_kd(0.8);
	phong->set_cs(0.75);
	phong->set_ks(0.15);
	phong->set_exp(50);
	phong->enable_recv_shadow(true);
	
	const char* ply_file_name = "../../res/plys/Horse2K.ply";
	TriangleMesh* mesh = new TriangleMesh(new Mesh, true);
	//mesh->read_flat_triangle(ply_file_name);
	mesh->read_smooth_triangle(ply_file_name);
	mesh->set_material(phong);
	mesh->setup_cells();
	mesh->enable_shadow(true);
	add_object(mesh);

	/*
	Grid* grid = new Grid;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Phong* phong_inst = (Phong*)phong->clone();
			phong_inst->set_cd(RGBColor(rand_float(), rand_float(), rand_float()));
			phong_inst->set_cs(RGBColor(rand_float(), rand_float(), rand_float()));

			Instance* mesh_inst = new Instance(mesh);
			mesh_inst->set_material(phong_inst);
			mesh_inst->translate(i * 1.5, 0, j * 1.5);
			mesh_inst->end_transform();
			//add_object(mesh_inst);
			grid->add_object(mesh_inst);
		}
	}

	grid->setup_cells();
	add_object(grid);
	*/

	Matte* matte_ptr2 = new Matte;			
	matte_ptr2->set_cd(RGBColor(1, 0.9, 0.6));
	matte_ptr2->set_ka(0.25); 
	matte_ptr2->set_kd(0.4);
	matte_ptr2->enable_recv_shadow(true);

	Plane* plane_ptr1 = new Plane(Point3D(0, -0.83, 0), Normal(0, 1, 0));  
	plane_ptr1->set_material(matte_ptr2);
	plane_ptr1->enable_shadow(true);
	add_object(plane_ptr1);
	
}

void World::build_reflective_test_scene()
{
	int num_samples = 256;

	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_max_depth(5);
	vp.set_samples(num_samples);

	tracer_ptr = new AreaLighting(this);

	PinHole* pinhole_ptr = new PinHole;
	pinhole_ptr->eye = Point3D(200, 50, 200);
	pinhole_ptr->lookat = Point3D(0, -0.5, 0);
	pinhole_ptr->d = 16000;  	
	pinhole_ptr->compute_uvw();     
	set_camera(pinhole_ptr);

	Jittered* sampler_ptr = new Jittered(num_samples);
	AmbientOccluder* occluder = new AmbientOccluder;
	occluder->set_radiance(1.0f);
	occluder->set_color(1.0f);
	occluder->set_min_amount(0.5f);
	occluder->set_sampler(sampler_ptr);
	set_ambient_light(occluder);

	Direction* directional_ptr = new Direction;
	directional_ptr->set_direction(Vector3D(-0.75, -1, 0.15));     
	directional_ptr->scale_radiance(4.5);  
	directional_ptr->set_cast_shadow(true);
	add_light(directional_ptr);

	Reflective* reflective = new Reflective;
	reflective->set_cr(1);
	reflective->set_kr(0.8);
	reflective->set_kd(0);
	reflective->set_cd(0);
	reflective->set_cs(0);
	reflective->set_ks(0);
	reflective->set_exp(100.0f);
	reflective->enable_recv_shadow(true);

	GlossyReflective* glossy_reflective = new GlossyReflective;
	glossy_reflective->set_cr(1);
	glossy_reflective->set_kr(0.8);
	glossy_reflective->set_kd(0);
	glossy_reflective->set_cd(0);
	glossy_reflective->set_cs(0);
	glossy_reflective->set_ks(0);
	glossy_reflective->set_exp(10.0f);
	glossy_reflective->set_sampler(new Jittered(num_samples));
	glossy_reflective->enable_recv_shadow(true);

	Phong* phong = new Phong;
	phong->set_cd(0.75);
	phong->set_ka(0.3);
	phong->set_kd(0.8);
	phong->set_cs(0.75);
	phong->set_ks(0.15);
	phong->set_exp(10);
	phong->enable_recv_shadow(true);

	const char* ply_file_name = "../../res/plys/Horse2K.ply";
	TriangleMesh* mesh = new TriangleMesh(new Mesh, true);
	//mesh->read_flat_triangle(ply_file_name);
	mesh->read_smooth_triangle(ply_file_name);
	mesh->set_material(phong);
	mesh->setup_cells();
	mesh->enable_shadow(true);
	add_object(mesh);
	
	//raytracer::Rectangle* rect = new raytracer::Rectangle(Point3D(0, -0.83, -2), Vector3D(3, 0, 0), Vector3D(0, 3, 0));
	//rect->set_material(reflective);
	//add_object(rect);
	
	Sphere* sphere0 = new Sphere(Point3D(1, 0, -2), 1);
	//sphere0->set_material(reflective);
	sphere0->set_material(glossy_reflective);
	add_object(sphere0);

	Matte* matte_ptr2 = new Matte;			
	matte_ptr2->set_cd(RGBColor(1, 0.9, 0.6));
	matte_ptr2->set_ka(0.25); 
	matte_ptr2->set_kd(0.4);
	matte_ptr2->enable_recv_shadow(true);

	Plane* plane_ptr1 = new Plane(Point3D(0, -0.83, 0), Normal(0, 1, 0));  
	plane_ptr1->set_material(matte_ptr2);
	plane_ptr1->enable_shadow(true);
	add_object(plane_ptr1);
}

void World::build_global_test_scene()
{
	//    int num_samples = 1;		// for Figure 26.7(a)
	//	int num_samples = 100;		// for Figure 26.7(b)
	//	int num_samples = 1000;		// for Figure 26.7(c)
		int num_samples = 10000;	// for Figure 26.7(d)

	vp.set_hres(300);	  		
	vp.set_vres(300);
	vp.set_samples(num_samples); 
	vp.set_max_depth(5);

	tracer_ptr = new GlobalTracer(this);

	AmbientOccluder* occluder = new AmbientOccluder;
	occluder->set_radiance(1.0f);
	occluder->set_color(1.0f);
	occluder->set_min_amount(0.0f);
	occluder->set_sampler(new Hammersley(num_samples));
	set_ambient_light(occluder);

	PinHole* pinhole_ptr = new PinHole;
	pinhole_ptr->eye = Point3D(27.6, 27.4, -80.0);
	pinhole_ptr->lookat = Point3D(27.6, 27.4, 0.0);
	pinhole_ptr->d = (400);      
	pinhole_ptr->compute_uvw();     
	set_camera(pinhole_ptr);


	Point3D p0;
	Vector3D a, b;
	Normal normal;

	// box dimensions

	double width 	= 55.28;   	// x direction
	double height 	= 54.88;  	// y direction
	double depth 	= 55.92;	// z direction


	// the ceiling light - doesn't need samples

	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->set_ce(RGBColor(1.0, 0.73, 0.4));
	emissive_ptr->set_ls(100);

	p0 = Point3D(21.3, height - 0.001, 22.7);
	a = Vector3D(0.0, 0.0, 10.5);
	b = Vector3D(13.0, 0.0, 0.0);
	normal = Normal(0.0, -1.0, 0.0);
	Hammersley* rectangle_sampler_ptr = new Hammersley(num_samples);
	raytracer::Rectangle* light_ptr = new raytracer::Rectangle(p0, b, a);
	light_ptr->set_material(emissive_ptr);
	light_ptr->set_sampler(rectangle_sampler_ptr);
	add_object(light_ptr);

	AreaLight* area_light_ptr = new AreaLight;
	area_light_ptr->set_object(light_ptr);
	area_light_ptr->set_cast_shadow(true);
	add_light(area_light_ptr);


	// left wall

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.0);
	matte_ptr1->set_kd(0.6); 
	matte_ptr1->set_cd(RGBColor(.57, 0.025, 0.025));	 // red
	matte_ptr1->set_sampler(new Hammersley(num_samples));

	p0 = Point3D(width, 0.0, 0.0);
	a = Vector3D(0.0, 0.0, depth);
	b = Vector3D(0.0, height, 0.0);
	normal = Normal(-1.0, 0.0, 0.0);
	rectangle_sampler_ptr = new Hammersley(num_samples);
	raytracer::Rectangle* left_wall_ptr = new raytracer::Rectangle(p0, a, b);
	left_wall_ptr->set_material(matte_ptr1);
	left_wall_ptr->set_sampler(rectangle_sampler_ptr);
	add_object(left_wall_ptr);


	// right wall

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(0.0);
	matte_ptr2->set_kd(0.6); 
	matte_ptr2->set_cd(RGBColor(.37, 0.59, 0.2));	 // green   from Photoshop
	matte_ptr2->set_sampler(new Hammersley(num_samples));

	p0 = Point3D(0.0, 0.0, 0.0);
	a = Vector3D(0.0, 0.0, depth);
	b = Vector3D(0.0, height, 0.0);
	normal = Normal(1.0, 0.0, 0.0);
	rectangle_sampler_ptr = new Hammersley(num_samples);
	raytracer::Rectangle* right_wall_ptr = new raytracer::Rectangle(p0, b, a);
	right_wall_ptr->set_material(matte_ptr2);
	right_wall_ptr->set_sampler(rectangle_sampler_ptr);
	add_object(right_wall_ptr);


	// back wall

	Matte* matte_ptr3 = new Matte;
	matte_ptr3->set_ka(0.0);
	matte_ptr3->set_kd(0.6); 
	matte_ptr3->set_cd(1.0);	 // white
	matte_ptr3->set_sampler(new Hammersley(num_samples));

	p0 = Point3D(0.0, 0.0, depth);
	a = Vector3D(width, 0.0, 0.0);
	b = Vector3D(0.0, height, 0.0);
	normal = Normal(0.0, 0.0, -1.0);
	raytracer::Rectangle* back_wall_ptr = new raytracer::Rectangle(p0, b, a);
	back_wall_ptr->set_material(matte_ptr3);
	back_wall_ptr->set_sampler(new Hammersley(num_samples));
	add_object(back_wall_ptr);


	// floor

	p0 = Point3D(0.0, 0.0, 0.0);
	a = Vector3D(0.0, 0.0, depth);
	b = Vector3D(width, 0.0, 0.0);
	normal = Normal(0.0, 1.0, 0.0);
	raytracer::Rectangle* floor_ptr = new raytracer::Rectangle(p0, a, b);
	floor_ptr->set_material(matte_ptr3);
	floor_ptr->set_sampler(new Hammersley(num_samples));
	add_object(floor_ptr);


	// ceiling

	p0 = Point3D(0.0, height, 0.0);
	a = Vector3D(0.0, 0.0, depth);
	b = Vector3D(width, 0.0, 0.0);
	normal = Normal(0.0, -1.0, 0.0);
	raytracer::Rectangle* ceiling_ptr = new raytracer::Rectangle(p0, b, a);
	ceiling_ptr->set_material(matte_ptr3);
	ceiling_ptr->set_sampler(new Hammersley(num_samples));
	add_object(ceiling_ptr);
}