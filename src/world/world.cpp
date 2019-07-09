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
		quit_render_tag(false),
		ambient_ptr(new Ambient)
{
	Ambient* ap = dynamic_cast<Ambient*>(ambient_ptr);
	if (ap)
		ap->set_color(0);
}

World::~World(void) {	
	
	if(tracer_ptr) {
		delete tracer_ptr;
		tracer_ptr = NULL;
	}				
}

void World::DisplayPixel(const int row, const int column, const RGBColor& raw_color,
							  RenderThread* paint_thread) const
{
	RGBColor mapped_color = raw_color;
	if (vp.GetGamutDisplay())
		mapped_color = ClampToColor(raw_color);
	else
		mapped_color = MaxToOne(raw_color);

	if (vp.GetGamma() != 1.0)
		mapped_color = mapped_color.PowC(vp.GetInvGamma());

	//have to start from max y coordinate to convert to screen coordinates
	int x = column;
	int y = vp.GetVres() - row - 1;

	paint_thread->setPixel(x, y, (int)(mapped_color.r * 255),
		(int)(mapped_color.g * 255),
		(int)(mapped_color.b * 255));
}

RGBColor
World::MaxToOne(const RGBColor& c) const  {
	float max_value = std::max(c.r, std::max(c.g, c.b));
	
	if (max_value > 1.0)
		return (c / max_value);
	else
		return (c);
}


// ------------------------------------------------------------------ clamp_to_color
// Set color to red if any component is greater than one

RGBColor
World::ClampToColor(const RGBColor& raw_color) const {
	RGBColor c(raw_color);
	
	if (raw_color.r > 1.0 || raw_color.g > 1.0 || raw_color.b > 1.0) {
		c.r = 1.0; c.g = 0.0; c.b = 0.0;
	}
		
	return (c);
}

void World::Build()
{
	srand((int)time(0));
	//this->build_ao_scene();
	//this->build_area_lights_scene();
	//this->build_env_light_scene();
	//this->build_instance_test_scene();
	//this->build_grid_test_scene();
	//this->build_triangle_mesh_test_scene();
	//this->build_reflective_test_scene();
	this->BuildGlobalTestScene();
	//this->BuildTransparentTestScene();
}

ShadeRec World::HitBareBonesObjects(const Ray& ray)
{
	ShadeRec sr(*this);
	double t;
	double tmin = INT_MAX;
	size_t num_objs = objects.size();

	for (size_t i = 0; i != num_objs; i++)
	{
		ShadeRec sr_stack(sr);	
		if (objects[i]->Hit(ray, t, sr) && (t < tmin))
		{
			sr.hit_an_object = true;
			tmin = t;
			sr.color = objects[i]->GetColor();
		}
		else
		{
			sr = sr_stack;
		}
	}
	return sr;
}

ShadeRec World::HitObject(const Ray& ray)
{
	ShadeRec sr(*this);
	double t;
	double tmin = INT_MAX;
	Normal normal;
	Point3D local_hit_point;
	size_t num_objects = objects.size();

	for (size_t i = 0; i != num_objects; i++)
	{
		if (objects[i]->Hit(ray, t, sr) && (t < tmin))
		{
			sr.hit_an_object = true;
			tmin = t;
			if (objects[i]->GetMaterial() == NULL)
				bool _debug = true;
			sr.material_ptr = objects[i]->GetMaterial();
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