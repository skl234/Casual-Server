#include "TCPListener.h"

constexpr byte g_backlog = 15;

CTCPListener::CTCPListener(const char* _addr, int _port)
	:m_socket(_addr, _port)
{

}

CTCPListener::~CTCPListener()
{
	m_socket.Close();
}

void CTCPListener::Close()
{
	m_socket.Close();
}

bool CTCPListener::Start()
{
	if (m_socket.Bind() == SOCKET_ERROR) return false;
	if (m_socket.Listen() == SOCKET_ERROR) return false;

	return true;
}

bool CTCPListener::Accept(SOCKET& _socket, SOCKADDR_IN& _sockAddr)
{
	_socket = m_socket.Accept(_sockAddr);
	if (_socket == INVALID_SOCKET) return false;
	return true;
}