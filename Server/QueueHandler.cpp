#include "QueueHandler.h"
#include "eTcpPacketType.h"
#include "eException.h"
CQueueHandler::CQueueHandler(CCircleQueue* _queue)
	:m_queue(_queue),
	m_buffTemp(new byte[_queue->GetCapacity()])
{
	
}

CQueueHandler::~CQueueHandler()
{
	if(m_queue) delete m_queue;
	if (m_buffTemp) delete[] m_buffTemp;
}

void CQueueHandler::TransByte(DWORD _byteTrans)
{
	m_queue->AddSize(_byteTrans);
}

CCircleQueue* CQueueHandler::GetQueue()
{
	return m_queue;
}

byte* CQueueHandler::GetPacket()
{
	size_t buffSize = m_queue->GetSize();
	
	u_short packetSize = 0;
	if (!m_queue->GetWord(&packetSize)) return nullptr;
	if (buffSize < packetSize) return nullptr;

	byte* packet = nullptr;
	if (packetSize <= m_queue->GetReadAbleSize())
	{
		packet = m_queue->GetReadPointer();
		m_queue->Pop(packetSize);
	}
	else
	{
		m_queue->Peek(m_buffTemp, packetSize);
		packet = m_buffTemp;
		m_queue->Pop(packetSize);
	}

	return packet;
}