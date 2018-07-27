#ifndef __WORLD_H__
#define __WORLD_H__

#include "viewplane.h"
#include "utilities/rgbcolor.h"
#include "geometricobjects/sphere.h"
#include <vector>

class Tracer;
class Light;
class Camera;
class RenderThread;


class World
{
public:
	ViewPlane					vp;
	RGBColor					background_color;
	Tracer*						tracer_ptr;
	Light*   					ambient_ptr;
	Camera*						camera_ptr;		
	Sphere 						sphere;		// for Chapter 3 only
	std::vector<GeometricObject*>	objects;		
	std::vector<Light*> 				lights;

	float						eye;	// eye point for prospective
	float						d;		// distance from eye point to viewplane
	bool						quit_render_tag;

	RenderThread* paintArea;

public:
	World();
	~World();

	void add_object(GeometricObject* object_ptr);
	void add_light(Light* light_ptr);
	void set_ambient_light(Light* ambient_light_ptr);
	void set_camera(Camera* camera_ptr);
	void build();
	void render_scene() const;
	void render_perspective() const;
	void open_window(const int hres, const int vres) const;

	void display_pixel(const int row, const int col, 
			const RGBColor& pixel_c) const;
	void quit_render() {quit_render_tag = true;};
	
	RGBColor max_to_one(const RGBColor& c) const;
	RGBColor clamp_to_color(const RGBColor& c) const;

	ShadeRec hit_bare_bones_objects(const Ray& ray);
	
	ShadeRec hit_object(const Ray& ray);

	// -- build samples --
protected:
	void build_ao_scene();
	void build_area_lights_scene();
	void build_env_light_scene();
	void build_instance_test_scene();
	void build_grid_test_scene();
	void build_triangle_mesh_test_scene();
	void build_reflective_test_scene();
	void build_global_test_scene();
	// -- --

private:
	void del_objects();
	void del_lights();
};


inline void 
World::add_object(GeometricObject* object_ptr) {  
	objects.push_back(object_ptr);	
}


inline void 
World::add_light(Light* light_ptr) {  
	lights.push_back(light_ptr);
}

inline void
World::set_ambient_light(Light* light_ptr) {
	ambient_ptr = light_ptr;
}

inline void
World::set_camera(Camera* c_ptr) {
	camera_ptr = c_ptr;
}


#endif