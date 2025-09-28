#pragma once
#include <set>
#include "Room.h"
#include "Page.h"

class CActRoomManager
{
private:
	CRITICAL_SECTION m_cs;
	std::set<CRoom*> m_roomList;

public:
	CActRoomManager();
	~CActRoomManager();

	void AddRoom(CRoom* _room);
	void EraseRoom(CRoom* _room);

	//return fail : Ȱ��ȭ�� �� 0��
	bool GetProfilesAndLastPageIndex(u_short& _pageIndex, 
		std::vector<ROOM_PROFILE>& _list, u_short& _lastPageIndex);

	CRoom* GetAccessibleRoom();
};