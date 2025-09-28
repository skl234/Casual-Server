#pragma once
#include "CircleQueue.h"
class CQueueHandler
{
private:
	CCircleQueue* m_queue;
	byte* const m_buffTemp;

public:
	CQueueHandler(CCircleQueue* _queue);
	~CQueueHandler();

	void TransByte(DWORD _byteTrans);
	CCircleQueue* GetQueue();
	byte* GetPacket();
};

