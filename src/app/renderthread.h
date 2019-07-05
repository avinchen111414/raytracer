#ifndef __RENDERTHREAD_H__
#define __RENDERTHREAD_H__

#include <wx/wx.h>

class World;
class RenderCanvas;
class RGBColor;

class RenderPixel
{
public:
	RenderPixel(int x, int y, int red, int green, int blue)
		: x(x), y(y), red(red), green(green), blue(blue) {}

public:
	int x, y;
	int red, green, blue;
};

class RenderTile
{
public:
	RenderTile(int left, int bottom, int right, int top):
		left(left), bottom(bottom), right(right), top(top)	
	{}
	RenderTile():
		left(0), bottom(0), right(0), top(0)
	{}

	int left;
	int bottom;
	int right;
	int top;
};

class RenderThread: public wxThread
{
public:
	RenderThread(int index, RenderCanvas* c, World* w): 
		wxThread(), m_world(w), m_canvas(c), m_index(index)
	{}

public:
	virtual void* Entry();
	virtual void OnExit();

public:
	// �������ռ���(row, column)����ɫת��Ϊ���ڿռ䲢����
	void RenderPixelColor(int row, int column, const RGBColor& color);

	inline int GetIndex() const { return m_index; }
	inline void SetWorld(World* world) { m_world = world; }

protected:
	// ��������ʾ��λ�ã���ɫֵ�Ž����棬�ȴ���Ⱦ
	void SetPixel(int x, int y, int red, int green, int blue);
	// �ѻ����m_pixels�ύ��UI�߳���Ⱦ
	void NotifyCanvas();

private:
	// �̱߳�ʶ
	int m_index;

	// ��Ⱦ���磬�����������ռ�����
	World* m_world;
	// ����
	RenderCanvas* m_canvas;
	// ����ģ�����Ⱦ�õ�������ɫ���ݣ��ռ���һ�������֪ͨUI�߳���Ⱦ
	std::vector<RenderPixel> m_pixels;
	// ���¶�ʱ������ʱ֪ͨUI�߳���Ⱦ
	wxStopWatch* m_timer;
	// �ϴθ���UI�̵߳�ʱ�䣬���m_timerʹ��
	long m_last_update_time;
	// ������Ⱦ�Ļ�����Χ
	RenderTile m_rendering_tile;


	const static int UPDATE_INTERVAL = 40;
};

#endif