#include "WorkerThread.h"
#include <stdio.h>
#include "User.h"
#include "eException.h"
CWorkerThread::CWorkerThread(HANDLE _completionPort)
	:m_completionPort(_completionPort)
{

}

CWorkerThread::~CWorkerThread()
{

}

void CWorkerThread::Run()
{
	DWORD byteTrans;
	WSAOVERLAPPED overlapped;
	void* completionKey;

	while (true)
	{
		if (!GetQueuedCompletionStatus(m_completionPort, &byteTrans, (PULONG_PTR)&completionKey, (LPOVERLAPPED*)&overlapped, INFINITE))
		{
			DWORD error = GetLastError();
			if (error == ERROR_NETNAME_DELETED)
			{
				Disconnect(reinterpret_cast<CUser*>(completionKey));
				continue;
			}
			else if (error == ERROR_CONNECTION_ABORTED)
			{
				continue;
			}
			else if (error == WSAECONNRESET)
			{
				Disconnect(reinterpret_cast<CUser*>(completionKey));
				continue;
			}

			printf("WorkerThread Error, Error Code : %d\n", error);

			continue;
		}


		if (completionKey == reinterpret_cast<void*>(g_IOCPExit)) break;
		if (byteTrans == 0)
		{
			Disconnect(reinterpret_cast<CUser*>(completionKey));
			continue;
		}
		try
		{
			reinterpret_cast<CConnection*>(completionKey)->OnRecv(byteTrans);
		}
		catch (eException _exception)
		{
			switch (_exception)
			{
			case eException::Error_QueueOverFlow:
				printf("Queue OverFlow\n");
				Disconnect(reinterpret_cast<CUser*>(completionKey));
				break;

			case eException::Error_QueueUnderFlow:
				printf("Queue UnderFlow\n");
				Disconnect(reinterpret_cast<CUser*>(completionKey));
				break;

			default:
				Disconnect(reinterpret_cast<CUser*>(completionKey));
				break;
			}
			continue;
		}

		if (!reinterpret_cast<CConnection*>(completionKey)->AsyncRecv())
		{
			Disconnect(reinterpret_cast<CUser*>(completionKey));
		}
	}
}

void CWorkerThread::Disconnect(CUser* _user)
{
	_user->Disconnect();
	delete _user;
}