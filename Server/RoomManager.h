#pragma once
#include <vector>
#include "Room.h"
#include "InactRoomManager.h"
#include "ActRoomManager.h"
class CRoomManager
{
private:
	std::vector<CRoom> m_roomList;

	CInactRoomManager m_inactRoomManager;
	CActRoomManager m_actRoomManager;
public:
	CRoomManager(size_t _capacity);
	~CRoomManager();

	void AddInactRoom(CRoom* _room);
	void AddActRoom(CRoom* _room);

	CRoom* GetRoom(unsigned int _number);
	CRoom* GetInactRoom();
	CRoom* GetAccessibleRoom();

	std::vector<CRoom>& GetRoomList();
	CInactRoomManager* GetInactRoomManager();
	CActRoomManager* GetActRoomManager();
};