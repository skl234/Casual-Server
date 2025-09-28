#include "ActRoomManager.h"

CActRoomManager::CActRoomManager()
{
	InitializeCriticalSection(&m_cs);
}

CActRoomManager::~CActRoomManager()
{
	DeleteCriticalSection(&m_cs);
}

void CActRoomManager::AddRoom(CRoom* _room)
{
	EnterCriticalSection(&m_cs);
	m_roomList.insert(_room);
	LeaveCriticalSection(&m_cs);
}

void CActRoomManager::EraseRoom(CRoom* _room)
{
	EnterCriticalSection(&m_cs);
	m_roomList.erase(_room);
	LeaveCriticalSection(&m_cs);
}

bool CActRoomManager::GetProfilesAndLastPageIndex(u_short& _pageIndex,
	std::vector<ROOM_PROFILE>& _list, u_short& _lastPageIndex)
{
	EnterCriticalSection(&m_cs);
	size_t roomListSize = m_roomList.size();
	if (roomListSize == 0)
	{
		LeaveCriticalSection(&m_cs);
		return false;
	}

	std::set<CRoom*>::iterator iter = m_roomList.begin();
	std::set<CRoom*>::iterator end = m_roomList.end();

	//pageIndex 할당
	size_t profileCapacity = _list.capacity();
	_lastPageIndex = (roomListSize - 1) / profileCapacity;
	if (_pageIndex > _lastPageIndex)
	{
		_pageIndex = _lastPageIndex;
	}

	for (u_int i = 0; i < _pageIndex * profileCapacity; ++i)
	{
		++iter;
	}
	//Profile 할당
	ROOM_PROFILE profile;
	for (iter; iter != end; ++iter)
	{
		if (_list.size() >= profileCapacity) break;

		(*iter)->GetProfile(profile);
		_list.push_back(profile);
	}

	LeaveCriticalSection(&m_cs);

	return true;
}

CRoom* CActRoomManager::GetAccessibleRoom()
{
	CRoom* room = nullptr;
	EnterCriticalSection(&m_cs);
	if (m_roomList.size() == 0) room = nullptr;
	else
	{
		std::set<CRoom*>::iterator iter = m_roomList.begin();
		std::set<CRoom*>::iterator end = m_roomList.end();
		ROOM_PROFILE profile;
		for (iter; iter != end; ++iter)
		{
			(*iter)->GetProfile(profile);
			if (profile.userMax - profile.userSize <= 0) continue;

			room = (*iter);
			break;
		}
	}
	LeaveCriticalSection(&m_cs);

	return room;
}