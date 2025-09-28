#include "RoomManager.h"

CRoomManager::CRoomManager(size_t _capacity)
{
	m_roomList.resize(_capacity);
	for (size_t i = 0; i < _capacity; ++i)
	{
		m_roomList[i].Initialize(i + 1, this);
	}

	m_inactRoomManager.AddRoom(m_roomList);
}

CRoomManager::~CRoomManager()
{
	
}

void CRoomManager::AddInactRoom(CRoom* _room)
{
	m_actRoomManager.EraseRoom(_room);
	m_inactRoomManager.AddRoom(_room);
}

void CRoomManager::AddActRoom(CRoom* _room)
{
	m_actRoomManager.AddRoom(_room);
}

CRoom* CRoomManager::GetRoom(unsigned int _number)
{
	if (_number <= 0 || _number > m_roomList.size()) return nullptr;
	return &m_roomList[_number - 1];
}

CRoom* CRoomManager::GetInactRoom()
{
	return m_inactRoomManager.PopRoom();
}

CRoom* CRoomManager::GetAccessibleRoom()
{
	return m_actRoomManager.GetAccessibleRoom();
}

std::vector<CRoom>& CRoomManager::GetRoomList()
{
	return m_roomList;
}

CInactRoomManager* CRoomManager::GetInactRoomManager()
{
	return &m_inactRoomManager;
}

CActRoomManager* CRoomManager::GetActRoomManager()
{
	return &m_actRoomManager;
}