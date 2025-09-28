#pragma once
#include "Lobby.h"
#include "RoomManager.h"
class CChannel
{
private:
	CUserManager* m_userManager;
	CLobby* m_lobby;

public:
	CChannel(u_short _capacity);
	~CChannel();

	CLobby* Enter(CUser* _user);
	void Leave(CUser* _user);

	CUserManager* GetUserManager();
};