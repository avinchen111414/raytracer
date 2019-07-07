#ifndef __THREADCONTEXT_H__
#define __THREADCONTEXT_H__

#include <map>
#include <Windows.h>

class ThreadContext
{
private:
	ThreadContext();
	ThreadContext(const ThreadContext&);
	ThreadContext& operator= (const ThreadContext&);

public:
	static ThreadContext& Instance();
	int GetNumProcesses() const { return m_num_processes;  };
	void RegisterThread(DWORD tid, int index);
	int GetCurrentThreadIndex() const;

private:
	int m_num_processes;
	std::map<DWORD, int> m_id2index;
};

#endif
