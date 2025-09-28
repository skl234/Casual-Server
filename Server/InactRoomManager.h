#pragma once
#include <deque>
#include <vector>
#include "Room.h"

class CInactRoomManager
{
private:
	CRITICAL_SECTION m_cs;
	std::deque<CRoom*> m_roomList;

public:
	CInactRoomManager();
	~CInactRoomManager();

	void AddRoom(std::vector<CRoom>& _roomList);
	void AddRoom(CRoom* _room);
	CRoom* PopRoom();
};