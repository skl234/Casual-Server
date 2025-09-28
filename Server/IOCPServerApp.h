#pragma once
#include "ServerApp.h"
#include "TCPListener.h"
#include "IOCP.h"
#include "Channel.h"
#include "UDPSocket.h"
#include "UDPRecvThread.h"

class CIOCPServerApp : public CServerApp
{
private:
	CTCPListener* m_listener;
	CIOCP* m_iocp;

	CUserManager* m_inServerUserManager;
	CChannel* m_channel;

	CUDPSocket* m_udpSocket;
	CUDPRecvThread* m_udpThread;

public:
	CIOCPServerApp();
	~CIOCPServerApp() override;

public:
	void Run() override;

private:
	void OnInit(const char* _addr, int _tcpPort, int _udpPort) override;
	void OnCleanup() override;

	void ResFailEnteredServer(CUser* _user);
	void ResSuccessEnteredServer(CUser* _user);
};