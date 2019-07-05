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
	// 将画布空间下(row, column)的颜色转换为窗口空间并缓存
	void RenderPixelColor(int row, int column, const RGBColor& color);

	inline int GetIndex() const { return m_index; }
	inline void SetWorld(World* world) { m_world = world; }

protected:
	// 将参数表示的位置，颜色值放进缓存，等待渲染
	void SetPixel(int x, int y, int red, int green, int blue);
	// 把缓存的m_pixels提交给UI线程渲染
	void NotifyCanvas();

private:
	// 线程标识
	int m_index;

	// 渲染世界，负责管理几何与空间数据
	World* m_world;
	// 画布
	RenderCanvas* m_canvas;
	// 缓存的，已渲染好的像素颜色数据，收集了一定量后会通知UI线程渲染
	std::vector<RenderPixel> m_pixels;
	// 更新定时器，定时通知UI线程渲染
	wxStopWatch* m_timer;
	// 上次更新UI线程的时间，配合m_timer使用
	long m_last_update_time;
	// 正在渲染的画布范围
	RenderTile m_rendering_tile;


	const static int UPDATE_INTERVAL = 40;
};

#endif