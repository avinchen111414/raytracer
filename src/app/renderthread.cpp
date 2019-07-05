#include "wxraytracer.h"
#include "tracer/tracer.h"
#include "lights/light.h"
#include "camera/camera.h"
#include "world/world.h"

#include "renderthread.h"

#include "camera/camera.h"
#include "camera/pinhole.h"
#include "utilities/ray.h"
#include "sampler/sampler.h"
#include "utilities/utility.h"

void* RenderThread::Entry()
{
	m_last_update_time = 0;
	m_timer = new wxStopWatch();

	while (m_canvas->PopTile(m_rendering_tile))
	{
		// 退出渲染
		if(TestDestroy())
			break;

		// 通过镜头去看世界
		m_world->camera_ptr->RenderScene(*m_world,
			m_rendering_tile, this);
	}

	return NULL;
}

void RenderThread::OnExit()
{
	// 可能还缓存着一些数据，退出先都提交掉
	NotifyCanvas();
	
	// 通知UI线程，渲染线程执行完毕
	wxCommandEvent event(wxEVT_RENDER, ID_RENDER_THREAD_COMPLETED);
	event.SetClientData(this);
	m_canvas->GetEventHandler()->AddPendingEvent(event);
}

void RenderThread::RenderPixelColor(int row, int column, const RGBColor& color)
{
	RGBColor mapped_color = color;
	if (m_world->vp.show_out_of_gamut)
		mapped_color = ClampToColor(color);
	else
		mapped_color = MaxToOne(color);

	if (m_world->vp.gamma != 1.0)
		mapped_color = mapped_color.powc(m_world->vp.inv_gamma);

	// UI画布的UV坐标以左上角为原点，V方向取反
	int x = column;
	int y = m_world->vp.vres - row - 1;

	SetPixel(x, y, (int)(mapped_color.r * 255),
		(int)(mapped_color.g * 255),
		(int)(mapped_color.b * 255));
}

void RenderThread::SetPixel(int x, int y, int red, int green, int blue)
{
	// 数据先缓存起来，等后面一起提交
	m_pixels.push_back(RenderPixel(x, y, red, green, blue));

	if(m_timer->Time() - m_last_update_time > UPDATE_INTERVAL)
	{
		// 提交数据
		NotifyCanvas();
	}

	// 检查下主线程是否要求销毁线程
	TestDestroy();
}

void RenderThread::NotifyCanvas()
{
	m_last_update_time = m_timer->Time();
	// 数据要拷贝出来提交
	std::vector<RenderPixel> *pixelsUpdate = new std::vector<RenderPixel>(m_pixels);
	m_pixels.clear();
	// 通过事件通知UI线程
	wxCommandEvent event(wxEVT_RENDER, ID_RENDER_NEWPIXEL);
	event.SetClientData(pixelsUpdate);
	m_canvas->GetEventHandler()->AddPendingEvent(event);
}
