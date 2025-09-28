#pragma once
#include "TCPSocket.h"

class CTCPListener
{
private:
	CTCPSocket m_socket;

public:
	CTCPListener(const char* _addr, int _port);
	~CTCPListener();

	void Close();
	bool Start();
	bool Accept(SOCKET& _socket, SOCKADDR_IN& _sockAddr);
};