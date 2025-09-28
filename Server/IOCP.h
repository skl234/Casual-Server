#pragma once
#include <WinSock2.h>
#include <vector>
#include "WorkerThread.h"

constexpr ULONG_PTR g_IOCPExit = reinterpret_cast<ULONG_PTR>(nullptr);

class CIOCP
{
private:
	HANDLE m_completionPort;
	std::vector<CWorkerThread*> m_threadList;

public:
	CIOCP(DWORD _threadCount = 0);
	~CIOCP();

	bool Add(HANDLE _handle, ULONG_PTR _completionKey);
};

//
// 생성자 , 파괴자 투머치
// 
//