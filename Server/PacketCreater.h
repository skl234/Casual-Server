#pragma once
#include "Packet_Type.h"
class CPacketCreater
{
private:

public:
	CPacketCreater();
	~CPacketCreater();

	static void Create(char* _out_packet, ePacket_Type _type,
		char* _source = nullptr, unsigned short _sourceSize = 0);
};