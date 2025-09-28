#include "TCPSocket.h"
#include <WS2tcpip.h>
#include "eException.h"
#include <stdio.h>
CTCPSocket::CTCPSocket(const char* _addr, int _port)
{
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		printf("TCP socket Func Error\n");
		throw eException::Fail_socketFunc;
	}

	memset(&m_sockAddr, NULL, sizeof(m_sockAddr));
	m_sockAddr.sin_family = AF_INET;
	if (!inet_pton(AF_INET, _addr, &m_sockAddr.sin_addr))
	{
		printf("TCP inet_pton Error\n");
		throw eException::Fail_inet_pton;
	}
	m_sockAddr.sin_port = htons(_port);
}

CTCPSocket::CTCPSocket(SOCKET _socket, SOCKADDR_IN& _addr)
	:m_socket(_socket),
	m_sockAddr(_addr)
{

}

CTCPSocket::~CTCPSocket()
{
	Close();
}

int CTCPSocket::ShutDown(u_short _sdType)
{
	return shutdown(m_socket, _sdType);
}

void CTCPSocket::Close()
{
	if (m_socket) { closesocket(m_socket); m_socket = NULL; }
}

int CTCPSocket::Bind()
{
	return bind(m_socket, reinterpret_cast<SOCKADDR*>(&m_sockAddr), sizeof(SOCKADDR_IN));
}

int CTCPSocket::Listen()
{
	return listen(m_socket, SOMAXCONN);
}

SOCKET CTCPSocket::Accept(SOCKADDR_IN& _sockAddr)
{
	int addrSize = sizeof(SOCKADDR_IN);
	memset(&_sockAddr, 0, addrSize);

	return accept(m_socket, reinterpret_cast<SOCKADDR*>(&_sockAddr), &addrSize);
}

int CTCPSocket::Send(byte* _packet, u_short _len)
{
	return send(m_socket, reinterpret_cast<char*>(_packet), _len, 0);
}

int CTCPSocket::Send(byte* _packet)
{
	return send(m_socket, reinterpret_cast<char*>(_packet), 
		*reinterpret_cast<u_short*>(_packet), 0);
}

int CTCPSocket::WsaRecv(WSABUF* _wsaBuf, LPWSAOVERLAPPED _overlapped, DWORD _flags)
{
	return WSARecv(m_socket, _wsaBuf, 1, NULL, &_flags, _overlapped, nullptr);
}

SOCKET CTCPSocket::GetSocket()
{
	return m_socket;
}

SOCKADDR_IN& CTCPSocket::GetAddr()
{
	return m_sockAddr;
}