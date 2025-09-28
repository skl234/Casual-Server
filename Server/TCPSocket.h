#pragma once
#include <WinSock2.h>
class CTCPSocket
{
protected:
	SOCKET m_socket;
	SOCKADDR_IN m_sockAddr;

public:
	CTCPSocket(const char* _addr, int _port);
	CTCPSocket(SOCKET _socket, SOCKADDR_IN& _addr);
	~CTCPSocket();

	int ShutDown(u_short _sdType);
	void Close();

	int Bind();
	int Listen();
	SOCKET Accept(SOCKADDR_IN& _sockAddr);

	int Send(byte* _packet, u_short _len);
	int Send(byte* _packet);
	int WsaRecv(WSABUF* _wsaBuf, LPWSAOVERLAPPED _overlapped, DWORD _flags);

	SOCKET GetSocket();
	SOCKADDR_IN& GetAddr();
};