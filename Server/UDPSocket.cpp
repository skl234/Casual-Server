#include "UDPSocket.h"
#include <WS2tcpip.h>
#include "eException.h"
#include <stdio.h>
CUDPSocket::CUDPSocket()
{
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		printf("UDP socket Func Error\n");
		throw eException::Fail_socketFunc;
	}

	memset(&m_sockAddr, 0, sizeof(m_sockAddr));
}

CUDPSocket::~CUDPSocket()
{
	Close();
}

void CUDPSocket::Close()
{
	if(m_socket) closesocket(m_socket); 
	m_socket = NULL;
}

int CUDPSocket::Bind(const char* _addr, int _port)
{
	m_sockAddr.sin_family = AF_INET;
	m_sockAddr.sin_port = htons(_port);
	if (!inet_pton(AF_INET, _addr, &m_sockAddr.sin_addr))
	{
		printf("UDP inet_pton Error\n");
		throw eException::Fail_inet_pton;
	}

	return bind(m_socket, reinterpret_cast<SOCKADDR*>(&m_sockAddr), sizeof(m_sockAddr));
}

int CUDPSocket::Send(void* _buff, int _size, SOCKADDR_IN& _to)
{
	return sendto(m_socket, reinterpret_cast<char*>(_buff), _size, 0, 
		reinterpret_cast<SOCKADDR*>(&_to), sizeof(_to));
}

int CUDPSocket::Recv(void* _buff, int _size, SOCKADDR_IN& _from)
{
	int addrSize = sizeof(_from);
	return recvfrom(m_socket, reinterpret_cast<char*>(_buff), _size, 0,
		reinterpret_cast<SOCKADDR*>(&_from), &addrSize);
}