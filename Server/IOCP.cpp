#include "IOCP.h"
#include "eException.h"
#include <Windows.h>

CIOCP::CIOCP(DWORD _threadCount)
{
	//완료포트 생성
	m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, _threadCount);
	if (!m_completionPort)
	{
		printf("CreateIoCompletionPort 실패\n");
		throw eException::Fail_CreateIpCompletionPort;
	}

	//WorkerThread 생성
	DWORD threadCount = 0;
	if (_threadCount == 0)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		m_threadList.reserve(si.dwNumberOfProcessors);
		threadCount = si.dwNumberOfProcessors;
	}
	else
	{
		m_threadList.reserve(_threadCount);
		threadCount = _threadCount;
	}

	CWorkerThread* thread = nullptr;
	for (DWORD i = 0; i < threadCount; ++i)
	{
		thread = new CWorkerThread(m_completionPort);
		if (!thread->Start()) printf("Worker Thread 시작 실패");
		m_threadList.push_back(thread);
	}
}

CIOCP::~CIOCP()
{
	if (m_completionPort)
	{
		int size = m_threadList.size();
		std::vector<HANDLE> threadHandleList;
		threadHandleList.reserve(size);

		//WorkerThread에 종료알림 보내기
		for (int i = 0; i < size; ++i)
		{
			threadHandleList.push_back(m_threadList[i]->GetHandle());
			PostQueuedCompletionStatus(m_completionPort, 0, g_IOCPExit, NULL);
		}

		//모든 WorkerThread 종료 대기
		WaitForMultipleObjects(static_cast<DWORD>(size), threadHandleList.data(), TRUE, INFINITE);

		//WorkerThread 객체 파괴
		for (int i = 0; i < size; ++i)
		{
			delete m_threadList[i];
		}

		//완료포트 닫기
		CloseHandle(m_completionPort);
	}

	printf("IOCP End\n");
}

bool CIOCP::Add(HANDLE _handle, ULONG_PTR _completionKey)
{
	if (!CreateIoCompletionPort(_handle, m_completionPort, _completionKey, 0))
		return false;

	return true;
}