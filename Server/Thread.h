#pragma once
#include <WinSock2.h>
class CThread
{
protected:
	HANDLE m_handle;
	unsigned int m_threadID;

public:
	CThread();
	virtual ~CThread();

	bool Start();
	HANDLE GetHandle();

protected:
	static unsigned int _stdcall ThreadFunc(void* _pArgs);
	virtual void Run() = 0;
};