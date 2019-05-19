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
	m_d(500),
	m_zoom(1.0f)
{}

PinHole::PinHole(const PinHole& c)
	:	Camera(c),
	m_d(c.m_d),
	m_zoom(c.m_zoom)
{}

PinHole::~PinHole() {}

void PinHole::RenderScene(const World& w, const RenderTile& tile, RenderThread* paint_thread)
{
	RGBColor	L;
	ViewPlane	vp(w.vp);	 								
	Ray			ray;
	int 		depth = 0;  
	Point2D 	pp;		// sample point on a pixel
	int n = (int)sqrt((float)vp.num_samples);
	Point2D		sp;

	vp.s /= m_zoom;
	ray.o = m_eye;

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

				ray.d = RayDirection(pp);
				L += w.tracer_ptr->trace_ray(ray, 0);
			}
			L /= (float)vp.num_samples;
			L *= m_exposure_time;
			w.display_pixel(r, c, L, paint_thread);
		}
	}
}

Vector3D PinHole::RayDirection(const Point2D& p) const
{
	Vector3D ray_dir = p.x * m_u + p.y * m_v - m_d * m_w;
	ray_dir.normalize();
	return ray_dir;
}