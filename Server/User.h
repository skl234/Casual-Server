#pragma once
#include "Connection.h"
#include "USER_PROFILE.h"
#include "Page.h"
#include "SlotState.h"
#include <vector>
#include "PLAYER_DATA.h"

class CLobby;
class CRoom;
class CGameSession;

class CUser : public CConnection
{
private:
	CMemoryStream m_sendBuffStream;

	CRITICAL_SECTION m_profileCs;
	USER_PROFILE m_profile;
	CLobby* m_lobby;
	CRoom* m_room;

public:
	CUser(SOCKET _socket, SOCKADDR_IN& _addr);
	~CUser() override;

	void GetProfile(USER_PROFILE& _profile);
	void Disconnect();

private:
	void HandlePacket(byte* _packet) override;

	//C2S
	void ReqCreateName(byte* _data);
	void ReqEnterChannel();
	void ReqEnterLobby();
	void ReqCreateRoom(byte* _data);
	void ReqEnterRoom(byte* _data);
	void ReqQuickEnterRoom();
	void ReqLeaveRoom();
	void ReqChangeReady();
	void ReqChangeCharacterType(byte* _data);
	void ReqUserPage(byte* _data);
	void ReqRoomPage(byte* _data);
	void ReqGameStart();
	void ReqGameData();
	void ReqUDPInfo(byte* _data);

	//S2C
	void ResCreateNameResult(bool _result);
	void ResEnterChannelResult(bool _result);
	void ResEnterLobbyResult(bool _result);
	void ResCreateRoomResult(bool _result);
	void ResEnterRoomResult(bool _result);
	void ResLeaveRoom();
	void ResIsReady();
	void ResChatLobby(byte* _data);
	void ResChatRoom(byte* _data);
	void ResRoomInfo();
	void ResUserPage(CPage& _page, u_short _lastPageIndex);
	void ResRoomPage(CPage& _page, u_short _lastPageIndex);
	void ResGameData(byte _index, eRole _role, std::vector<PLAYER_DATA>& _list);
};