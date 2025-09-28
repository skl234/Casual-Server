#include "Thread.h"
#include <process.h>
CThread::CThread()
	:m_handle(NULL),
	m_threadID(NULL)
{

}

CThread::~CThread()
{
	if(m_handle) CloseHandle(m_handle);
}

bool CThread::Start()
{
	m_handle = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, ThreadFunc, this, 0, &m_threadID));
	if (!m_handle) return false;

	return true;
}

HANDLE CThread::GetHandle()
{
	return m_handle;
}

unsigned int _stdcall CThread::ThreadFunc(void* _pArgs)
{
	CThread* pThread = reinterpret_cast<CThread*>(_pArgs);

	pThread->Run();

	return 0;
}