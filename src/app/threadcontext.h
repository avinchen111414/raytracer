#ifndef __THREADCONTEXT_H__
#define __THREADCONTEXT_H__

#include <vector>

class RenderThread;
class ThreadContext
{
private:
	ThreadContext() {};

public:
	// ��C++11�У���̬�����Ķ���ʱ�̰߳�ȫ��
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