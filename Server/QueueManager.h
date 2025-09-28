#pragma once
#include "CircleQueue.h"
class CQueueManager
{
private:
	CCircleQueue* m_queue;
	byte* const m_buffTemp;

public:
	CQueueManager(CCircleQueue* _queue);
	~CQueueManager();

	CCircleQueue* GetQueue();
	byte* GetPacket(DWORD _byteTrans);
};

