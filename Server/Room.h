#pragma once
#include <vector>
#include "ROOM_PROFILE.h"
#include "SlotState.h"
#include "MemoryStream.h"
#include "PLAYER_DATA.h"
#include <vector>
#include "SlotState.h"
#include "GameStartHelper.h"

class CUser;
class CRoomManager;

class CRoom
{
private:
	CRoomManager* m_roomManager;

	CRITICAL_SECTION m_profileCs;
	ROOM_PROFILE m_profile;

	//SlotList
	CRITICAL_SECTION m_slotListCS;
	std::vector<CSlotState> m_slotList;
	byte m_currentUserSize;

	//게임시작 도우미(플레이어정보, UDP정보)
	CGameStartHelper m_helper;

public:
	CRoom();
	~CRoom();

	void Initialize(u_short _num, CRoomManager* _roomManager);

	void Open(CUser* _host, byte* _roomName, byte _roomNameSize);
	bool Enter(CUser* _user);
	void Leave(CUser* _user);
	void ChangeReady(CUser* _user);
	void UnReady(CUser* _user);
	void ChangeChracterType(CUser* _user, eCharacterType _type);
	void StartGame();
	void SetUDPAddr(CUser* _user, UDPADDRESS_INFO& _info);
	void EndGame(byte* _packet);

	void SendPacketToAll(byte* _packet);
	void SendPacketToOther(CUser* _sender, byte* _packet);
	void SendPacketToOther(byte _senderIndex, byte* _packet);
	void SendPacketToHost(byte* _packet);

	void GetProfile(ROOM_PROFILE& _profile);
	void WriteRoomInfo(CUser* _user, CMemoryStream& _stream);
	void GetGameData(CUser* _user, byte& _index, eRole& _role, std::vector<PLAYER_DATA>& _list);

private:
	int  AddUser(CUser* _user, eRole _role);
	byte EraseUser(CUser* _user, eRole& _role);
	byte UpdateHost();
	bool IsAllReady();
	byte GetIndex(CUser* _user);
	byte GetHostIndex();

	void BroadcastEnterdUser(byte _index);
	void BroadcastLeavedUser(byte _index);
	void BroadcastNewHost(byte _index);
	void BroadcastReadyState(byte _index, bool _ready);
	void BroadcastCharacterType(byte _index, eCharacterType _type);
	void BroadcastStartGame(bool _result);
	void BroadcastUDPAddrList();
};