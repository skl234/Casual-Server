#pragma once
#include <WinSock2.h>
class CUDPSocket
{
private:
	SOCKET m_socket;
	SOCKADDR_IN m_sockAddr;

public:
	CUDPSocket();
	~CUDPSocket();

	void Close();
	int Bind(const char* _addr, int _port);
	int Send(void* _buff, int _size, SOCKADDR_IN& _to);
	int Recv(void* _buff, int _size, SOCKADDR_IN& _from);
};
