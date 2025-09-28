#include "Connection.h"
#include "UserManager.h"
#include "eTcpPacketType.h"
#include "eException.h"

constexpr size_t g_buffSize = 1024;

CConnection::CConnection(SOCKET _socket, SOCKADDR_IN& _addr)
	:m_socket(_socket, _addr),
	m_queueHandler(new CCircleQueue(g_buffSize))
{
	memset(&m_wsaInfo, 0, sizeof(m_wsaInfo));
}

CConnection::~CConnection()
{
	
}

bool CConnection::RegisterIOCP(CIOCP* _iocp)
{
	if (!_iocp->Add(reinterpret_cast<HANDLE>(m_socket.GetSocket()), reinterpret_cast<ULONG_PTR>(this)))
	{
		printf("Connection IOCP 등록 실패! Error Code : %d \n", WSAGetLastError()); return false;
	}
	if (!AsyncRecv())
	{
		printf("Connection AsyncRecv 실패! Error Code : %d \n", WSAGetLastError()); return false;
	}

	return true;
}

void CConnection::OnRecv(DWORD _byteTrans)
{
	m_queueHandler.TransByte(_byteTrans);

	byte* packet = nullptr;
	while (true)
	{
		packet = m_queueHandler.GetPacket();
		if (!packet) break;

		HandlePacket(packet);
	}
}

bool CConnection::AsyncRecv()
{
	CCircleQueue* queue = m_queueHandler.GetQueue();
	m_wsaInfo.wsaBuf.len = queue->GetWriteAbleSize();
	m_wsaInfo.wsaBuf.buf = reinterpret_cast<char*>(queue->GetWritePointer());
	if (m_socket.WsaRecv(&m_wsaInfo.wsaBuf, &m_wsaInfo.overlapped, m_wsaInfo.flags) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING) return false;
	}
	return true;
}

bool CConnection::SendPacket(byte* _packet, u_short _len)
{
	if (m_socket.Send(_packet, _len) == SOCKET_ERROR) return false;
	return true;
}

bool CConnection::SendPacket(byte* _packet)
{
	if (m_socket.Send(_packet) == SOCKET_ERROR) return false;
	return true;
}