#include "GameStartHelper.h"
#include <algorithm>
#include <random>
#include "User.h"
CGameStartHelper::CGameStartHelper()
{
	InitializeCriticalSection(&m_udpAddrCS);
	m_isReceivingUDP = false;
}

CGameStartHelper::~CGameStartHelper()
{
	DeleteCriticalSection(&m_udpAddrCS);
}

void CGameStartHelper::Clear()
{
	m_playerList.clear();
	m_udpAddrList.clear();
	m_isReceivingUDP = false;
}

void CGameStartHelper::Init(std::vector<CSlotState>& _list)
{
	int size = _list.size();
	std::vector<byte> redTeamIndexList;
	std::vector<byte> blueTeamIndexList;
	ShuffleTeam(size, redTeamIndexList, blueTeamIndexList);

	m_playerList.resize(size);
	int redSize = redTeamIndexList.size();
	for (int i = 0; i < redSize; ++i)
	{
		byte index = redTeamIndexList[i];
		USER_PROFILE profile;
		_list[index].GetUser()->GetProfile(profile);

		m_playerList[index].index = index;
		m_playerList[index].nameBuffLength = profile.nameBuffLength;
		memcpy(m_playerList[index].nameBuff, profile.nameBuff, profile.nameBuffLength);
		m_playerList[index].role = _list[index].GetRole();
		m_playerList[index].charType = _list[index].GetCharType();
		m_playerList[index].color = eTeamColor::Red;
	}

	int blueSize = blueTeamIndexList.size();
	for (int i = 0; i < blueSize; ++i)
	{
		byte index = blueTeamIndexList[i];
		USER_PROFILE profile;
		_list[index].GetUser()->GetProfile(profile);

		m_playerList[index].index = index;
		m_playerList[index].nameBuffLength = profile.nameBuffLength;
		memcpy(m_playerList[index].nameBuff, profile.nameBuff, profile.nameBuffLength);
		m_playerList[index].role = _list[index].GetRole();
		m_playerList[index].charType = _list[index].GetCharType();
		m_playerList[index].color = eTeamColor::Blue;
	}

	m_isReceivingUDP = true;
}

int CGameStartHelper::SetUDPAddressInfo(byte _index, UDPADDRESS_INFO& _addrInfo)
{
	int size;
	EnterCriticalSection(&m_udpAddrCS);
	//printf("%u , %u, %u, %u\n", _addrInfo.sourceIp, _addrInfo.sourcePort,
		//_addrInfo.destinationIp, _addrInfo.destinationPort);

	unsigned char bytes[4];
	bytes[0] = (_addrInfo.sourceIp & 0xFF);
	bytes[1] = (_addrInfo.sourceIp >> 8) & 0xFF;
	bytes[2] = (_addrInfo.sourceIp >> 16) & 0xFF;
	bytes[3] = (_addrInfo.sourceIp >> 24) & 0xFF;
	printf("private addr : %u.%u.%u.%u, %u\n", bytes[0], bytes[1], bytes[2], bytes[3], _addrInfo.sourcePort);

	bytes[0] = (_addrInfo.destinationIp & 0xFF);
	bytes[1] = (_addrInfo.destinationIp >> 8) & 0xFF;
	bytes[2] = (_addrInfo.destinationIp >> 16) & 0xFF;
	bytes[3] = (_addrInfo.destinationIp >> 24) & 0xFF;
	printf("public addr : %u.%u.%u.%u, %u\n", bytes[0], bytes[1], bytes[2], bytes[3], _addrInfo.destinationPort);


	m_udpAddrList.insert({ _index, _addrInfo });
	size = m_udpAddrList.size();
	LeaveCriticalSection(&m_udpAddrCS);

	return size;
}

bool CGameStartHelper::IsReceivingUDP()
{
	return m_isReceivingUDP;
}

void CGameStartHelper::EarseIndex(byte _index)
{
	EnterCriticalSection(&m_udpAddrCS);
	m_udpAddrList.erase(_index);
	LeaveCriticalSection(&m_udpAddrCS);
}

void CGameStartHelper::ShuffleTeam(int _size, std::vector<byte>& _redTeam, std::vector<byte>& _blueTeam)
{
	std::vector<byte> m_indexList;		 m_indexList.reserve(_size);
	for (int i = 0; i < _size; ++i) m_indexList.push_back(i);

	std::random_device rd;
	std::shuffle(m_indexList.begin(), m_indexList.end(), std::default_random_engine(rd()));

	int size = m_indexList.size();
	for (int i = 0; i < size; ++i)
	{
		if (i % 2 == 0) _redTeam.push_back(m_indexList[i]);
		else			_blueTeam.push_back(m_indexList[i]);
	}
}

std::vector<PLAYER_DATA>& CGameStartHelper::GetPlayerDataList()
{
	return m_playerList;
}

std::map<byte, UDPADDRESS_INFO>& CGameStartHelper::GetUdpAddrInfoList()
{
	return m_udpAddrList;
}

int CGameStartHelper::GetUDPAddrSize()
{
	int size;
	EnterCriticalSection(&m_udpAddrCS);
	size = m_udpAddrList.size();
	LeaveCriticalSection(&m_udpAddrCS);

	return size;
}