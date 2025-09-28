#pragma once
#include "Thread.h"
#include "TCPListener.h"
class CServerAppStopThread : public CThread
{
private:
	CTCPListener& m_listener;

public:
	CServerAppStopThread(CTCPListener& _listener);
	~CServerAppStopThread() override;

protected:
	void Run() override;
};