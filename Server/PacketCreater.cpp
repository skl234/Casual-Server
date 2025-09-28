#include "PacketCreater.h"
CPacketCreater::CPacketCreater()
{

}

CPacketCreater::~CPacketCreater()
{

}

void CPacketCreater::Create(char* _out_packet, ePacket_Type _type,
	char* _source, unsigned short _sourceSize)
{
	char* packetTemp = _out_packet;
	if (_source == nullptr)
	{
		*reinterpret_cast<unsigned short*>(packetTemp) = HEADER_SIZE;
		packetTemp += sizeof(unsigned short);
		*reinterpret_cast<ePacket_Type*>(packetTemp) = _type;
		return;
	}
}