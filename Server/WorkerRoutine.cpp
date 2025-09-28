#include "WorkerRoutine.h"
#include <stdio.h>
CWorkerRoutine::CWorkerRoutine(CIOCP* _pIOCP)
	:m_pIOCP(_pIOCP)
{

}

CWorkerRoutine::~CWorkerRoutine()
{

}

void CWorkerRoutine::Run()
{
	DWORD byteTrans;
	WSAOVERLAPPED overlapped;
	void* completionKey;

	while (true)
	{
		if (!m_pIOCP->WaitIOCompletion(&byteTrans, (PULONG_PTR)&completionKey, (LPOVERLAPPED*)&overlapped))
		{
			if(!StopRoutine(GetLastError(), completionKey)) continue;

			break;
		}

		OnIOCompletion(byteTrans, completionKey);
	}
}