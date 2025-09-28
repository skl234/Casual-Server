#include "ServerApp.h"
#include <WinSock2.h>
#include "eException.h"
#include <stdio.h>
CServerApp::CServerApp()
{

}

CServerApp::~CServerApp()
{
	
}

void CServerApp::Initialize(const char* _addr, int _tcpPort, int _udpPort)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup Error\n");
		throw eException::Fail_WSAStartup;
	}
	OnInit(_addr, _tcpPort, _udpPort);
}

void CServerApp::Cleanup()
{
	OnCleanup();
	WSACleanup();
}