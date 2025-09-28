#pragma once
#include "Thread.h"
#include "UDPSocket.h"
class CUDPRecvThread : public CThread
{
private:
	CUDPSocket& m_socket;

public:
	CUDPRecvThread(CUDPSocket& _socket);
	~CUDPRecvThread() override;

protected:
	void Run() override;

private:
	void OnRecvHello(SOCKADDR_IN& _from);
};