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

void PinHole::RenderScene(World& w, const RenderTile& tile, RenderThread* paint_thread)
{
	RGBColor	L;
	//ViewPlane	vp(w.vp);	 								
	ViewPlane& vp = w.vp;
	Ray			ray;
	int 		depth = 0;  
	Point2D 	pp;		// sample point on a pixel
	int n = (int)sqrt((float)vp.GetNumSamples());
	Point2D		sp;

	float pixel_size = vp.GetPixelSize();
	pixel_size /= m_zoom;

	ray.o = m_eye;

	for (int r = tile.bottom; r < tile.top; r++)			// up
	{
		if (w.quit_render_tag)
			break;

		for (int c = tile.left; c < tile.right; c++) {		// across
			if (w.quit_render_tag)
				break;

			L = RGBColor(0.0f); 

			int num_samples = vp.GetNumSamples();
			for (int i = 0; i != num_samples; i++)
			{
				if (w.quit_render_tag)
					break;

				sp = vp.GetSampler()->SampleUnitSquare();
				pp.x = (float)(pixel_size * (c - 0.5 * (vp.GetHres() - 1) + sp.x));
				pp.y = (float)(pixel_size * (r - 0.5 * (vp.GetVres() - 1) + sp.y));

				ray.d = RayDirection(pp);
				L += w.tracer_ptr->trace_ray(ray, 0);
			}
			L /= (float)vp.GetNumSamples();
			L *= m_exposure_time;
			w.DisplayPixel(r, c, L, paint_thread);
		}
	}
}

Vector3D PinHole::RayDirection(const Point2D& p) const
{
	Vector3D ray_dir = p.x * m_u + p.y * m_v - m_d * m_w;
	ray_dir.Normalize();
	return ray_dir;
}