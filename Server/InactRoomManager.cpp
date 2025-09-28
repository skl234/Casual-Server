#include "InactRoomManager.h"
#include "Room.h"
CInactRoomManager::CInactRoomManager()
{
	InitializeCriticalSection(&m_cs);
}

CInactRoomManager::~CInactRoomManager()
{
	DeleteCriticalSection(&m_cs);
}

void CInactRoomManager::AddRoom(std::vector<CRoom>& _roomList)
{
	EnterCriticalSection(&m_cs);
	size_t size = _roomList.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_roomList.push_back(&_roomList[i]);
	}
	LeaveCriticalSection(&m_cs);
}

void CInactRoomManager::AddRoom(CRoom* _room)
{
	EnterCriticalSection(&m_cs);
	m_roomList.push_back(_room);
	LeaveCriticalSection(&m_cs);
}

CRoom* CInactRoomManager::PopRoom()
{
	CRoom* room = nullptr;
	EnterCriticalSection(&m_cs);
	if (m_roomList.empty() == false)
	{
		room = m_roomList.front();
		m_roomList.pop_front();
	}
	LeaveCriticalSection(&m_cs);
	return room;
}