#pragma once
#include "TCPSocket.h"
#include "QueueHandler.h"
#include "WSAINFO.h"
#include "IOCP.h"
class CConnection
{
protected:
	CTCPSocket m_socket;
	WSAINFO m_wsaInfo;
	CQueueHandler m_queueHandler;

public:
	CConnection(SOCKET _socket, SOCKADDR_IN& _addr);
	virtual ~CConnection();

	bool RegisterIOCP(CIOCP* _iocp);

	void OnRecv(DWORD _byteTrans);
	bool AsyncRecv();
	bool SendPacket(byte* _packet, u_short _len);
	bool SendPacket(byte* _packet);

	CTCPSocket& GetSocket() { return m_socket; }

protected:
	virtual void HandlePacket(byte* _packet) {}
};