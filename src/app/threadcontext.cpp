#include <windows.h>

#include "utilities/rgbcolor.h"
#include "renderthread.h"
#include "threadcontext.h"

std::vector<unsigned int> ThreadContext::m_threads;

void ThreadContext::RegisterRenderThread(const RenderThread& thread)
{
	m_threads.push_back(thread.GetId());
}

unsigned int ThreadContext::GetCurrentThread()
{
	return GetCurrentThreadId();
}
