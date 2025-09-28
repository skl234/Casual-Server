#pragma once
#include "Thread.h"
class CUser;
class CWorkerThread : public CThread
{
protected:
	HANDLE m_completionPort;

public:
	CWorkerThread(HANDLE _completionPort);
	~CWorkerThread() override;

protected:
	void Run() override;
	void Disconnect(CUser* _user);
};