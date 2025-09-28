#include "UDPRecvThread.h"
#include <stdio.h>
#include "eUdpPacketType.h"
#include "MemoryStream.h"
#include "SmartBuffer.h"
constexpr u_short g_buffSize = 1024;
CUDPRecvThread::CUDPRecvThread(CUDPSocket& _socket)
	:m_socket(_socket)
{
	
}

CUDPRecvThread::~CUDPRecvThread()
{

}

void CUDPRecvThread::Run()
{
	SOCKADDR_IN from; memset(&from, 0, sizeof(from));
	byte recvBuff[g_buffSize];
	int recvSize = 0;
	eUdpPacketType type = eUdpPacketType::Begin;

	while (true) 
	{
		recvSize = m_socket.Recv(recvBuff, g_buffSize, from);
		if (recvSize == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error == WSAECONNRESET) continue;
			// UDP에서 해당 에러가 뜨는건 ICMP메시지를 받았을 경우
			// 동일한 공유기를 사용할시 ICMP메시지를 차단하지 않기 때문에 이 에러가 뜸
			// 해당 오류일때는 아무런 행동을 하지 않는다.

			else if (error == WSAEINTR) printf("Closed UDPSocket, RecvThread Break\n");
			else printf("UDPRecv Error %d \n", error);

			break;
		}

		type = *reinterpret_cast<eUdpPacketType*>(recvBuff);
		if (type == eUdpPacketType::C2S_Hello)
		{
			OnRecvHello(from);
		}
		else if (type == eUdpPacketType::C2S_KeepMapping) { };
	}
}

void CUDPRecvThread::OnRecvHello(SOCKADDR_IN& _from)
{
	CMemoryStream stream(24);
	stream.Write(static_cast<u_short>(eUdpPacketType::S2C_HelloResponse));
	stream.Write(_from.sin_addr.S_un.S_addr);
	stream.Write(ntohs(_from.sin_port));

	m_socket.Send(stream.GetBuff(), stream.GetSize(), _from);
}