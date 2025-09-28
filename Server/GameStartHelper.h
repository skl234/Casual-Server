#pragma once
#include <vector>
#include <WinSock2.h>
#include "UDPAddressInfo.h"
#include "PLAYER_DATA.h"
#include "SlotState.h"
#include <map>
class CGameStartHelper
{
private:
	std::vector<PLAYER_DATA> m_playerList;

	CRITICAL_SECTION m_udpAddrCS;
	std::map<byte, UDPADDRESS_INFO> m_udpAddrList;
	bool m_isReceivingUDP;

public:
	CGameStartHelper();
	~CGameStartHelper();

	void Clear();
	void Init(std::vector<CSlotState>& _list);
	int SetUDPAddressInfo(byte _index, UDPADDRESS_INFO& _addrInfo);
	bool IsReceivingUDP();
	void EarseIndex(byte _index);
	
	std::vector<PLAYER_DATA>& GetPlayerDataList();
	std::map<byte, UDPADDRESS_INFO>& GetUdpAddrInfoList();
	int GetUDPAddrSize();

private:
	void ShuffleTeam(int _size, std::vector<byte>& _redTeam, std::vector<byte>& _blueTeam);
};