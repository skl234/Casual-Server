#pragma once
#include <WinSock2.h>
class CTimer
{
private:
	DWORD m_prevTick;
	unsigned int m_interval;

public:
	CTimer();
	~CTimer();

	void Init(unsigned int _miliSeconds);
	void Reset();
	bool IsTimeUp();
};