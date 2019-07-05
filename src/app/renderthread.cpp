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
		// �˳���Ⱦ
		if(TestDestroy())
			break;

		// ͨ����ͷȥ������
		m_world->camera_ptr->RenderScene(*m_world,
			m_rendering_tile, this);
	}

	return NULL;
}

void RenderThread::OnExit()
{
	// ���ܻ�������һЩ���ݣ��˳��ȶ��ύ��
	NotifyCanvas();
	
	// ֪ͨUI�̣߳���Ⱦ�߳�ִ�����
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

	// UI������UV���������Ͻ�Ϊԭ�㣬V����ȡ��
	int x = column;
	int y = m_world->vp.vres - row - 1;

	SetPixel(x, y, (int)(mapped_color.r * 255),
		(int)(mapped_color.g * 255),
		(int)(mapped_color.b * 255));
}

void RenderThread::SetPixel(int x, int y, int red, int green, int blue)
{
	// �����Ȼ����������Ⱥ���һ���ύ
	m_pixels.push_back(RenderPixel(x, y, red, green, blue));

	if(m_timer->Time() - m_last_update_time > UPDATE_INTERVAL)
	{
		// �ύ����
		NotifyCanvas();
	}

	// ��������߳��Ƿ�Ҫ�������߳�
	TestDestroy();
}

void RenderThread::NotifyCanvas()
{
	m_last_update_time = m_timer->Time();
	// ����Ҫ���������ύ
	std::vector<RenderPixel> *pixelsUpdate = new std::vector<RenderPixel>(m_pixels);
	m_pixels.clear();
	// ͨ���¼�֪ͨUI�߳�
	wxCommandEvent event(wxEVT_RENDER, ID_RENDER_NEWPIXEL);
	event.SetClientData(pixelsUpdate);
	m_canvas->GetEventHandler()->AddPendingEvent(event);
}
