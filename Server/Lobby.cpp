#include "Lobby.h"
CLobby::CLobby(u_short _capacity, CUserManager* _channelUserManager)
	:m_inLobbyUserManager(_capacity),
	m_roomManager(_capacity),
	m_userPageManager(_capacity, _channelUserManager),
	m_roomPageManager(_capacity, &m_roomManager)
{

}

CLobby::~CLobby()
{

}

bool CLobby::Enter(CUser* _user)
{
	return m_inLobbyUserManager.Add(_user);
}

void CLobby::Leave(CUser* _user)
{
	m_inLobbyUserManager.Erase(_user);
}

void CLobby::SendPacketToAll(byte* _packet)
{
	m_inLobbyUserManager.SendPacketToAll(_packet);
}

u_short CLobby::GetUserPage(u_short _index, CPage& _page)
{
	return m_userPageManager.GetPage(_index, _page);
}

u_short CLobby::GetRoomPage(u_short _index, CPage& _page)
{
	return m_roomPageManager.GetPage(_index, _page);
}

CRoom* CLobby::GetInactRoom()
{
	return m_roomManager.GetInactRoom();
}

CRoom* CLobby::GetRoom(u_short _number)
{
	return m_roomManager.GetRoom(_number);
}

CRoom* CLobby::GetRoom()
{
	return m_roomManager.GetAccessibleRoom();
}