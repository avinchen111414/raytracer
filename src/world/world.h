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

public:
	World();
	~World();

	void AddObject(GeometricObject* object_ptr);
	void AddLight(Light* light_ptr);
	void SetAmbientLight(Light* ambient_light_ptr);
	void SetCamera(Camera* camera_ptr);
	void Build();

	void DisplayPixel(const int row, const int col,
		const RGBColor& pixel_c, RenderThread* paint_thread) const;

	void QuitRender() {quit_render_tag = true;};
	
	RGBColor MaxToOne(const RGBColor& c) const;
	RGBColor ClampToColor(const RGBColor& c) const;

	ShadeRec HitBareBonesObjects(const Ray& ray);
	
	ShadeRec HitObject(const Ray& ray);

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
	void build_transparent_test_scene();
	// -- --

private:
	void DelObjects();
	void DelLights();
};


inline void 
World::AddObject(GeometricObject* object_ptr) {  
	objects.push_back(object_ptr);	
}


inline void 
World::AddLight(Light* light_ptr) {  
	lights.push_back(light_ptr);
}

inline void
World::SetAmbientLight(Light* light_ptr) {
	ambient_ptr = light_ptr;
}

inline void
World::SetCamera(Camera* c_ptr) {
	camera_ptr = c_ptr;
}


#endif