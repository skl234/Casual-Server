#include "Room.h"
#include "User.h"
#include "RoomManager.h"
#include "eTcpPacketType.h"
#include <algorithm>
#include <random>
#include "SmartBuffer.h"

constexpr byte g_maxUser = 4;
constexpr u_short g_buffSize = 64;
constexpr byte g_invalidIndex = 128;

CRoom::CRoom()
	:m_roomManager(nullptr)
{
	InitializeCriticalSection(&m_profileCs);
	InitializeCriticalSection(&m_slotListCS);
	m_profile.userMax = g_maxUser;
	m_slotList.resize(g_maxUser);
	m_currentUserSize = 0;
}

CRoom::~CRoom()
{
	DeleteCriticalSection(&m_slotListCS);
	DeleteCriticalSection(&m_profileCs);
}

void CRoom::Initialize(u_short _num, CRoomManager* _roomManager)
{
	m_profile.number = _num;
	m_roomManager = _roomManager;
}

void CRoom::Open(CUser* _host, byte* _roomName, byte _roomNameSize)
{
	m_profile.nameBuffLength = _roomNameSize;
	memcpy(m_profile.nameBuff, _roomName, _roomNameSize);
	m_profile.state = eRoomState::WAITING;

	AddUser(_host, eRole::HOST);

	m_roomManager->AddActRoom(this);
}

bool CRoom::Enter(CUser* _user)
{
	EnterCriticalSection(&m_profileCs);
	if (m_profile.state == eRoomState::PLAYING)
	{
		LeaveCriticalSection(&m_profileCs);
		return false;
	}
	LeaveCriticalSection(&m_profileCs);

	int index = 0;
	EnterCriticalSection(&m_slotListCS);
	if (m_currentUserSize == 0)
	{
		LeaveCriticalSection(&m_slotListCS);
		return false;
	}

	index = AddUser(_user, eRole::GUEST);
	if (index == -1)
	{
		LeaveCriticalSection(&m_slotListCS);
		return false;
	}

	BroadcastEnterdUser(static_cast<byte>(index));
	LeaveCriticalSection(&m_slotListCS);

	return true;
}

void CRoom::Leave(CUser* _user)
{
	eRole leaveRole = eRole::GUEST;
	int leaveIndex = 0;
	EnterCriticalSection(&m_slotListCS);

	leaveIndex = EraseUser(_user, leaveRole);
	if (m_currentUserSize > 0)
	{
		if (m_helper.IsReceivingUDP())
		{
			m_helper.EarseIndex(leaveIndex);
			if (m_helper.GetUDPAddrSize() == m_currentUserSize)
			{
				BroadcastUDPAddrList();
			}
		}

		BroadcastLeavedUser(static_cast<byte>(leaveIndex));

		if (leaveRole == eRole::HOST)
		{
			int hostIndex = UpdateHost();
			BroadcastNewHost(static_cast<byte>(hostIndex));
		}
	}
	else if (m_currentUserSize == 0)
	{
		m_helper.Clear();
		m_roomManager->AddInactRoom(this);
	}

	LeaveCriticalSection(&m_slotListCS);
}

void CRoom::ChangeReady(CUser* _user)
{
	EnterCriticalSection(&m_slotListCS);

	byte index = GetIndex(_user);
	if (index == GetHostIndex())
	{
		LeaveCriticalSection(&m_slotListCS);
		return;
	}
	m_slotList[index].ChangeReady();
	BroadcastReadyState(index, m_slotList[index].GetReady());

	LeaveCriticalSection(&m_slotListCS);
}

void CRoom::UnReady(CUser* _user)
{
	EnterCriticalSection(&m_slotListCS);

	byte index = GetIndex(_user);
	m_slotList[index].UnReady();
	BroadcastReadyState(index, false);

	LeaveCriticalSection(&m_slotListCS);
}

void CRoom::ChangeChracterType(CUser* _user, eCharacterType _type)
{
	EnterCriticalSection(&m_slotListCS);

	byte index = GetIndex(_user);
	m_slotList[index].SetCharacterType(_type);
	BroadcastCharacterType(index, m_slotList[index].GetCharType());

	LeaveCriticalSection(&m_slotListCS);
}

void CRoom::StartGame()
{
	EnterCriticalSection(&m_slotListCS);
	if (IsAllReady())
	{
		EnterCriticalSection(&m_profileCs);
		m_profile.state = eRoomState::PLAYING;
		LeaveCriticalSection(&m_profileCs);

		for (byte i = 0; i < g_maxUser; ++i)
		{
			m_slotList[i].UnReady();
		}

		m_helper.Init(m_slotList);

		BroadcastStartGame(true);
	}
	else BroadcastStartGame(false);

	LeaveCriticalSection(&m_slotListCS);
}

void CRoom::SetUDPAddr(CUser* _user, UDPADDRESS_INFO& _info)
{
	EnterCriticalSection(&m_slotListCS);
	int count = m_helper.SetUDPAddressInfo(GetIndex(_user), _info);
	if (m_currentUserSize == count) BroadcastUDPAddrList();
	printf("UDP 받았고 현재유저%d, 카운트%d\n", m_currentUserSize, count);
	LeaveCriticalSection(&m_slotListCS);
}

void CRoom::EndGame(byte* _packet)
{
	EnterCriticalSection(&m_profileCs);
	m_profile.state = eRoomState::WAITING;
	LeaveCriticalSection(&m_profileCs);

	SendPacketToAll(_packet);
}

void CRoom::SendPacketToAll(byte* _packet)
{
	EnterCriticalSection(&m_slotListCS);
	for (byte i = 0; i < g_maxUser; ++i)
	{
		if (m_slotList[i].IsEmpty()) continue;
		m_slotList[i].GetUser()->SendPacket(_packet);
	}
	LeaveCriticalSection(&m_slotListCS);
}

void CRoom::SendPacketToOther(CUser* _sender, byte* _packet)
{
	EnterCriticalSection(&m_slotListCS);
	for (byte i = 0; i < g_maxUser; ++i)
	{
		if (m_slotList[i].IsEmpty() || m_slotList[i].GetUser() == _sender) continue;
		m_slotList[i].GetUser()->SendPacket(_packet);
	}
	LeaveCriticalSection(&m_slotListCS);
}

void CRoom::SendPacketToOther(byte _senderIndex, byte* _packet)
{
	EnterCriticalSection(&m_slotListCS);
	for (byte i = 0; i < g_maxUser; ++i)
	{
		if (m_slotList[i].IsEmpty() || i == _senderIndex) continue;
		m_slotList[i].GetUser()->SendPacket(_packet);
	}
	LeaveCriticalSection(&m_slotListCS);
}

void CRoom::SendPacketToHost(byte* _packet)
{
	EnterCriticalSection(&m_slotListCS);
	for (byte i = 0; i < g_maxUser; ++i)
	{
		if (m_slotList[i].GetRole() != eRole::HOST) continue;
		
		m_slotList[i].GetUser()->SendPacket(_packet);
		break;
	}
	LeaveCriticalSection(&m_slotListCS);
}

void CRoom::GetProfile(ROOM_PROFILE& _profile)
{
	EnterCriticalSection(&m_profileCs);
	EnterCriticalSection(&m_slotListCS);
	m_profile.userSize = m_currentUserSize;
	_profile = m_profile;
	LeaveCriticalSection(&m_slotListCS);
	LeaveCriticalSection(&m_profileCs);
}

void CRoom::WriteRoomInfo(CUser* _user, CMemoryStream& _stream)
{
	_stream.Write(m_profile.number);
	_stream.Write(m_profile.nameBuffLength);
	_stream.Write(m_profile.nameBuff, m_profile.nameBuffLength);

	EnterCriticalSection(&m_slotListCS);
	_stream.Write(GetIndex(_user));
	_stream.Write(m_currentUserSize);

	USER_PROFILE userProfile;
	for (byte i = 0; i < g_maxUser; ++i)
	{
		if (m_slotList[i].IsEmpty()) continue;
		m_slotList[i].GetUser()->GetProfile(userProfile);

		_stream.Write(i);
		_stream.Write(userProfile.nameBuffLength);
		_stream.Write(userProfile.nameBuff, userProfile.nameBuffLength);
		_stream.Write(static_cast<byte>(m_slotList[i].GetReady()));
		_stream.Write(static_cast<byte>(m_slotList[i].GetRole()));
		_stream.Write(static_cast<byte>(m_slotList[i].GetCharType()));
	}

	LeaveCriticalSection(&m_slotListCS);
}

void CRoom::GetGameData(CUser* _user, byte& _index, eRole& _role, std::vector<PLAYER_DATA>& _list)
{
	_index = GetIndex(_user);
	_role = m_slotList[_index].GetRole();
	_list = m_helper.GetPlayerDataList();
}

int CRoom::AddUser(CUser* _user, eRole _role)
{
	int index = -1;
	int size = m_slotList.size();
	for (int i = 0; i < size; ++i)
	{
		if (m_slotList[i].IsEmpty())
		{
			m_slotList[i].SetUser(_user, _role);
			index = i;
			break;
		}
	}

	if (index != -1) m_currentUserSize += 1;

	return index;
}

byte CRoom::EraseUser(CUser* _user, eRole& _role)
{
	byte index = 0;
	for (byte i = 0; i < g_maxUser; ++i)
	{
		if (m_slotList[i].GetUser() == _user)
		{
			_role = m_slotList[i].GetRole();
			m_slotList[i].Clear();
			index = i;
			break;
		}
	}
	m_currentUserSize -= 1;

	return index;
}

byte CRoom::UpdateHost()
{
	byte index = 0;
	for (int i = 0; i < g_maxUser; ++i)
	{
		if (!m_slotList[i].IsEmpty())
		{
			m_slotList[i].UnReady();
			m_slotList[i].SetRole(eRole::HOST);
			index = i;
			break;
		}
	}
	return index;
}

bool CRoom::IsAllReady()
{
	if (m_currentUserSize < g_maxUser) return false;

	for (byte i = 0; i < g_maxUser; ++i)
	{
		if (m_slotList[i].GetRole() == eRole::HOST) continue;
		if (m_slotList[i].GetReady()) continue;

		return false;
	}

	return true;
}

byte CRoom::GetIndex(CUser* _user)
{
	byte index = 0;
	for (index = 0; index < g_maxUser; ++index)
	{
		if (m_slotList[index].GetUser() == _user) break;
	}
	return index;
}

byte CRoom::GetHostIndex()
{
	byte index = 0;
	for (index = 0; index < g_maxUser; ++index)
	{
		if (m_slotList[index].IsEmpty()) continue;
		if (m_slotList[index].GetRole() != eRole::HOST) continue;
		
		break;
	}
	return index;
}

void CRoom::BroadcastEnterdUser(byte _index)
{
	USER_PROFILE userProfile;
	m_slotList[_index].GetUser()->GetProfile(userProfile);

	CMemoryStream memoryStream(g_buffSize);
	memoryStream.Write(static_cast<u_short>(0));
	memoryStream.Write(static_cast<u_short>(eTcpPacketType::S2C_EnteredRoomUser));
	memoryStream.Write(_index);
	memoryStream.Write(userProfile.nameBuffLength);
	memoryStream.Write(userProfile.nameBuff, userProfile.nameBuffLength);
	memoryStream.Write(static_cast<byte>(m_slotList[_index].GetCharType()));
	memoryStream.WriteSizeToFront();

	SendPacketToOther(_index, memoryStream.GetBuff());
}

void CRoom::BroadcastLeavedUser(byte _index)
{
	CMemoryStream memoryStream(g_buffSize);
	memoryStream.Write(static_cast<u_short>(g_headerSize + sizeof(byte)));
	memoryStream.Write(static_cast<u_short>(eTcpPacketType::S2C_LeavedRoomUser));
	memoryStream.Write(_index);

	SendPacketToOther(_index, memoryStream.GetBuff());
}

void CRoom::BroadcastNewHost(byte _index)
{
	CMemoryStream memoryStream(g_buffSize);
	memoryStream.Write(static_cast<u_short>(g_headerSize + sizeof(byte)));
	memoryStream.Write(static_cast<u_short>(eTcpPacketType::S2C_NewHostIndex));
	memoryStream.Write(_index);

	SendPacketToAll(memoryStream.GetBuff());
}

void CRoom::BroadcastReadyState(byte _index, bool _ready)
{
	CMemoryStream memoryStream(g_buffSize);
	memoryStream.Write(static_cast<u_short>(g_headerSize + (sizeof(byte) *2)));
	memoryStream.Write(static_cast<u_short>(eTcpPacketType::S2C_ChangeReady));
	memoryStream.Write(_index);
	memoryStream.Write(static_cast<byte>(_ready));

	SendPacketToAll(memoryStream.GetBuff());
}

void CRoom::BroadcastCharacterType(byte _index, eCharacterType _type)
{
	CMemoryStream memoryStream(g_buffSize);
	memoryStream.Write(static_cast<u_short>(g_headerSize + (sizeof(byte) * 2)));
	memoryStream.Write(static_cast<u_short>(eTcpPacketType::S2C_ChangeCharacterType));
	memoryStream.Write(_index);
	memoryStream.Write(static_cast<byte>(_type));

	SendPacketToAll(memoryStream.GetBuff());
}

void CRoom::BroadcastStartGame(bool _result)
{
	CMemoryStream memoryStream(g_buffSize);
	memoryStream.Write(static_cast<u_short>(g_headerSize + sizeof(byte)));
	memoryStream.Write(static_cast<u_short>(eTcpPacketType::S2C_GameStart));
	memoryStream.Write(static_cast<byte>(_result));

	if (_result == false) SendPacketToHost(memoryStream.GetBuff());
	else SendPacketToAll(memoryStream.GetBuff());
}

void CRoom::BroadcastUDPAddrList()
{
	std::map<byte, UDPADDRESS_INFO>& list = m_helper.GetUdpAddrInfoList();

	CMemoryStream memoryStream(g_buffSize);
	memoryStream.Write(static_cast<u_short>(g_headerSize));
	memoryStream.Write(static_cast<u_short>(eTcpPacketType::S2C_UDPAddressInfo));

	int size = list.size();
	memoryStream.Write(static_cast<byte>(size));

	std::map<byte, UDPADDRESS_INFO>::iterator iter = list.begin();
	std::map<byte, UDPADDRESS_INFO>::iterator end = list.end();
	for (iter; iter != end; ++iter)
	{
		memoryStream.Write(iter->second.sourceIp);
		memoryStream.Write(iter->second.sourcePort);
		memoryStream.Write(iter->second.destinationIp);
		memoryStream.Write(iter->second.destinationPort);
	}
	memoryStream.WriteSizeToFront();

	SendPacketToAll(memoryStream.GetBuff());

	m_helper.Clear();
}