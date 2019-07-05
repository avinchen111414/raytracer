#ifndef __THREADCONTEXT_H__
#define __THREADCONTEXT_H__

#include <vector>

class RenderThread;
class ThreadContext
{
private:
	ThreadContext() {};

public:
	// 在C++11中，静态变量的定义时线程安全的
	static ThreadContext& GetInstance()
	{
		static ThreadContext instance;
		return instance;
	}

	static void RegisterRenderThread(const RenderThread& thread);
	inline static std::vector<unsigned int>& GetThreads() { return m_threads; }
	static unsigned int GetCurrentThread();

private:
	static std::vector<unsigned int> m_threads;
};

#endif