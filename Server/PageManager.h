#pragma once
#include <vector>
#include "Page.h"
#include "Timer.h"

class CPageManager
{
protected:
	CRITICAL_SECTION m_cs;
	std::vector<CTimer> m_timerList;
	
	std::vector<CPage*> m_pageList;
	CPage* m_defaultPage;
	u_short m_lastPageIndex;

public:
	CPageManager(u_short _capacity);
	CPageManager(u_short _capacity, unsigned int _updateCycleSeconds);
	virtual ~CPageManager();

	//마지막 페이지의 Index를 return
	u_short GetPage(u_short _index, CPage& _page);

protected:
	virtual bool Update(u_short& _index) = 0;
	bool IsUpdated(u_short _index);
};