#pragma once
#include "UserManager.h"
#include "UserPageManager.h"
#include "RoomPageManager.h"
class CLobby
{
private:
	CUserManager m_inLobbyUserManager;
	CRoomManager m_roomManager;
	CUserPageManager m_userPageManager;
	CRoomPageManager m_roomPageManager;

public:
	CLobby(u_short _capacity, CUserManager* _channelUserManager);
	~CLobby();

	bool Enter(CUser* _user);
	void Leave(CUser* _user);
	void SendPacketToAll(byte* _packet);
	u_short GetUserPage(u_short _index, CPage& _page);
	u_short GetRoomPage(u_short _index, CPage& _page);
	CRoom* GetInactRoom();
	CRoom* GetRoom(u_short _number);
	CRoom* GetRoom();
};