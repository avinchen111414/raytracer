#include "pinhole.h"
#include "utilities/rgbcolor.h"
#include "world/viewplane.h"
#include "utilities/ray.h"
#include "world/world.h"
#include "utilities/point2d.h"
#include "tracer/tracer.h"
#include "sampler/sampler.h"
#include "app/wxraytracer.h"

PinHole::PinHole()
	:	Camera(),
	d(500),
	zoom(1.0f)
{}

PinHole::PinHole(const PinHole& c)
	:	Camera(c),
	d(c.d),
	zoom(c.zoom)
{}

PinHole::~PinHole() {}

void PinHole::render_scene(const World& w, const RenderTile& tile, RenderThread* paint_thread)
{
	RGBColor	L;
	ViewPlane	vp(w.vp);	 								
	Ray			ray;
	int 		depth = 0;  
	Point2D 	pp;		// sample point on a pixel
	int n = (int)sqrt((float)vp.num_samples);
	Point2D		sp;

	vp.s /= zoom;
	ray.o = eye;

	for (int r = tile.bottom; r < tile.top; r++)			// up
	{
		if (w.quit_render_tag)
			break;

		for (int c = tile.left; c < tile.right; c++) {		// across
			if (w.quit_render_tag)
				break;

			if (r == 350 && c == 200)
				bool _break = true;

			L = RGBColor(0.0f); 

			for (int i = 0; i != vp.num_samples; i++)
			{
				if (w.quit_render_tag)
					break;

				sp = vp.sampler_ptr->sample_unit_square();
				pp.x = (float)(vp.s * (c - 0.5 * (vp.hres - 1) + sp.x));
				pp.y = (float)(vp.s * (r - 0.5 * (vp.vres - 1) + sp.y));

				ray.d = ray_direction(pp);
				L += w.tracer_ptr->trace_ray(ray, 0);
			}
			L /= (float)vp.num_samples;
			L *= exposure_time;
			w.display_pixel(r, c, L, paint_thread);
		}
	}
}

Vector3D PinHole::ray_direction(const Point2D& p) const
{
	Vector3D ray_dir = p.x * u + p.y * v - d * w;
	ray_dir.normalize();
	return ray_dir;
}