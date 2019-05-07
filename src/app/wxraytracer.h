#ifndef __WXRAYTRACER_H__
#define __WXRAYTRACER_H__

#include <vector>
#include <wx/wx.h>
#include "world/world.h"

class wxraytracerFrame;
class RenderCanvas;

class wxraytracerapp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();
	virtual void SetStatusText(const wxString&  text, int number = 0);

private:
	wxraytracerFrame* frame;
	DECLARE_EVENT_TABLE()
};

class wxraytracerFrame : public wxFrame
{
public:
	wxraytracerFrame(const wxPoint& pos, const wxSize& size);
   
	//event handlers
	void OnQuit( wxCommandEvent& event );
	void OnOpenFile( wxCommandEvent& event );
	void OnSaveFile( wxCommandEvent& event );
	void OnRenderStart( wxCommandEvent& event );
	void OnRenderCompleted( wxCommandEvent& event );
	void OnRenderPause( wxCommandEvent& event );
	void OnRenderResume( wxCommandEvent& event );

private:
	RenderCanvas* canvas; //where the rendering takes place
	wxString currentPath; //for file dialogues
	DECLARE_EVENT_TABLE()
};

enum
{
	Menu_File_Quit = 100,
	Menu_File_Open,
	Menu_File_Save,
	Menu_Render_Start,
	Menu_Render_Pause,
	Menu_Render_Resume
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

class RenderCanvas: public wxScrolledWindow
{
public:
	RenderCanvas(wxWindow *parent);
	virtual ~RenderCanvas(void);
    
	void SetImage(wxImage& image);
	wxImage GetImage(void);
   
	virtual void OnDraw(wxDC& dc);
	void renderStart(void);
	void renderPause(void);
	void renderResume(void);
	void OnRenderCompleted( wxCommandEvent& event );
	void OnTimerUpdate( wxTimerEvent& event );
	void OnNewPixel( wxCommandEvent& event );

	void GenerateTiles();
	bool PopTile(RenderTile& tile);

	void CreatePainters();

protected:
	wxBitmap *m_image;
	World* w;

private:

	wxMutex tiles_mutex;
	std::vector<RenderTile> tiles;

	std::vector<RenderThread*> painters;
	DWORD num_task_completed;

	wxStopWatch* timer;
	long pixelsRendered;
	long pixelsToRender;
	wxTimer updateTimer;
   
	DECLARE_EVENT_TABLE()
};

class RenderPixel
{
public:
	RenderPixel(int x, int y, int red, int green, int blue);

public:
	int x, y;
	int red, green, blue;
};

DECLARE_EVENT_TYPE(wxEVT_RENDER, -1)
#define ID_RENDER_COMPLETED 100
#define ID_RENDER_NEWPIXEL  101
#define ID_RENDER_UPDATE    102
#define ID_RENDER_THREAD_COMPLETED 103

class RenderThread : public wxThread
{
public:
	RenderThread(RenderCanvas* c, World* w) : wxThread(), m_world(w), m_canvas(c), m_break_thread(false){}
	virtual void *Entry();
	virtual void OnExit();
	virtual void SetPixel(int x, int y, int red, int green, int blue);
	void BreakThread();

private:

	void FetchTile();
	void NotifyCanvas();

	World* m_world;
	RenderCanvas* m_canvas;

	std::vector<RenderPixel*> m_pixels;
	wxStopWatch* m_timer;
	long m_lastUpdateTime;

	RenderTile m_working_tile;
	bool m_break_thread;
};

#endif