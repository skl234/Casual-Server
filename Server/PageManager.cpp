#include "PageManager.h"
#include "Page.h"

constexpr unsigned int g_updateInterval = 2000;

CPageManager::CPageManager(u_short _capacity)
	:m_defaultPage(nullptr),
	m_lastPageIndex(0)
{
	InitializeCriticalSection(&m_cs);
	m_pageList.resize(_capacity);
	m_timerList.resize(_capacity);
	for (u_short i = 0; i < _capacity; ++i)
		m_timerList[i].Init(g_updateInterval);
}

CPageManager::CPageManager(u_short _capacity, unsigned int _updateCycleSeconds)
	:m_defaultPage(nullptr),
	m_lastPageIndex(0)
{
	InitializeCriticalSection(&m_cs);
	m_pageList.resize(_capacity);
	m_timerList.resize(_capacity);
	for (u_short i = 0; i < _capacity; ++i)
		m_timerList[i].Init(_updateCycleSeconds);
}

CPageManager::~CPageManager()
{
	DeleteCriticalSection(&m_cs);
}

//마지막 페이지의 인덱스를 return
u_short CPageManager::GetPage(u_short _index, CPage& _page)
{
	EnterCriticalSection(&m_cs);
	if (!IsUpdated(_index))
	{
		if (!Update(_index))
		{
			_page = *m_defaultPage;
			LeaveCriticalSection(&m_cs);
			return 0;
		}
	}

	u_short lastPage = m_lastPageIndex;
	_page = *m_pageList[_index];
	LeaveCriticalSection(&m_cs);

	return lastPage;
}

bool CPageManager::IsUpdated(u_short _index)
{
	if (!m_timerList[_index].IsTimeUp()) return true;

	return false;
}