#include "QueueManager.h"
#include "ePacket_Type.h"
#include "eException.h"
CQueueManager::CQueueManager(CCircleQueue* _queue)
	:m_queue(_queue),
	m_buffTemp(new byte[_queue->GetCapacity()])
{
	
}

CQueueManager::~CQueueManager()
{
	delete m_queue;
	if (m_buffTemp) delete[] m_buffTemp;
}

CCircleQueue* CQueueManager::GetQueue()
{
	return m_queue;
}

byte* CQueueManager::GetPacket(DWORD _byteTrans)
{
	size_t buffSize = m_queue->AddSize(_byteTrans);
	
	WORD packetSize = 0;
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