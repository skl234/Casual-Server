#include "PacketManager.h"
CPacketManager::CPacketManager(CCircleBuff* _recvBuff)
	:m_packet(_recvBuff)
{
	unsigned int capacity = m_packet->GetCapacity();
	m_packetTemp = new char[capacity];
}

CPacketManager::~CPacketManager()
{
	delete[] m_packetTemp;
}

bool CPacketManager::IsPacket(char** _out)
{
	PACKET_HEADER header;
	if (!m_packet->GetDWORD(reinterpret_cast<unsigned long*>(&header)))
	{
		*_out = nullptr;
		return false;
	}
	if (!m_packet->GetSize() < header.m_packetSize)
	{
		*_out = nullptr;
		return false;
	}

	if (m_packet->GetDirectReadAbleSize() >= header.m_packetSize)
	{
		*_out = m_packet->GetFront();
	}
	else
	{
		m_packet->Peek(m_packetTemp, header.m_packetSize);
		*_out = m_packetTemp;
	}

	return true;
}

bool CPacketManager::PopPacket()
{
	PACKET_HEADER header;
	if (!m_packet->GetDWORD(reinterpret_cast<unsigned long*>(&header)))
	{
		return false;
	}
	if (!m_packet->Pop(header.m_packetSize))
	{
		return false;
	}
	return true;
}

unsigned short CPacketManager::Create(char* _packet, ePacket_Type _type,
	char* _source, unsigned short _sourceSize)
{
	PACKET_HEADER header = { 0, ePacket_Type::Begin };
	char* packetTemp = _packet;
	if (_source == nullptr)
	{
		header.m_packetSize = HEADER_SIZE;
		header.m_packetType = _type;
		*reinterpret_cast<PACKET_HEADER*>(packetTemp) = header;
	}

	return header.m_packetSize;
}