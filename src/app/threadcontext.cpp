#include <wx/wx.h>
#include "threadcontext.h"

ThreadContext::ThreadContext()
{
#if defined(WIN32)
	SYSTEM_INFO system_info;
	::GetSystemInfo(&system_info);
	m_num_processes = system_info.dwNumberOfProcessors;
#else
	m_num_processes = 1;
#endif
}

ThreadContext & ThreadContext::Instance()
{
	static ThreadContext instance;
	return instance;
}

void ThreadContext::RegisterThread(DWORD tid, int index)
{
	m_id2index.insert(std::make_pair(tid, index));
}

int ThreadContext::GetCurrentThreadIndex() const
{
	DWORD current_thread_id = GetCurrentThreadId();
	assert(m_id2index.find(current_thread_id) != m_id2index.end());
	return m_id2index.at(current_thread_id);
}
