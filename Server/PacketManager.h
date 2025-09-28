#pragma once
#include "CircleBuff.h"
#include "PACKET_HEADER.h"
class CPacketManager
{
private:
	CCircleBuff* m_packet;
	char* m_packetTemp;
	
public:
	CPacketManager(CCircleBuff* _recvBuff);
	~CPacketManager();

	bool IsPacket(char** _out);
	bool PopPacket();

	unsigned short Create(char* _out, ePacket_Type _type,
		char* _source = nullptr, unsigned short _sourceSize = 0);
};