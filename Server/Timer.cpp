#include "Timer.h"
#pragma comment(lib, "Winmm.lib")
CTimer::CTimer()
	:m_prevTick(0),
	m_interval(0)
{

}

CTimer::~CTimer()
{

}

void CTimer::Init(unsigned int _miliSeconds)
{
	m_interval = _miliSeconds;
	m_prevTick = timeGetTime();
}

void CTimer::Reset()
{
	m_prevTick = timeGetTime();
}

bool CTimer::IsTimeUp()
{
	if (timeGetTime() - m_prevTick < m_interval)
	{
		return false;
	}
	return true;
}