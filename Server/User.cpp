#include "User.h"
#include "eTcpPacketType.h"
#include "ServerResourceProvider.h"
#include "SmartBuffer.h"
#include "UDPAddressInfo.h"

constexpr size_t g_buffSize = 1024;

CUser::CUser(SOCKET _socket, SOCKADDR_IN& _addr) :
	CConnection(_socket, _addr),
	m_sendBuffStream(g_buffSize),
	m_lobby(nullptr),
	m_room(nullptr)
{
	InitializeCriticalSection(&m_profileCs);
}

CUser::~CUser()
{
	DeleteCriticalSection(&m_profileCs);
	m_socket.ShutDown(SD_BOTH);
	m_socket.Close();
}

void CUser::GetProfile(USER_PROFILE& _profile)
{
	EnterCriticalSection(&m_profileCs);
	_profile = m_profile;
	LeaveCriticalSection(&m_profileCs);
}

void CUser::Disconnect()
{
	printf("%d : Disconnect\n", (int)m_socket.GetSocket());
	//if (!m_lobby) return;

	if (m_lobby)
	{
		if (m_room) 
		{ 
			m_room->UnReady(this);
			m_room->Leave(this);
			m_room = nullptr; 
		}
		m_lobby->Leave(this);
		CServerResourceProvider::GetInstance()->GetChannel()->Leave(this);
	}
	CServerResourceProvider::GetInstance()->GetServerUserManager()->Erase(this);

	m_socket.ShutDown(SD_BOTH);
}

void CUser::HandlePacket(byte* _packet)
{
	byte* packetTemp = _packet;
	u_short size = CSmartBuffer::ReadWord(&packetTemp);
	eTcpPacketType type = static_cast<eTcpPacketType>(CSmartBuffer::ReadWord(&packetTemp));
	
	switch (type)
	{
	case eTcpPacketType::C2S_CreateName:
		ReqCreateName(packetTemp);
		break;

	case eTcpPacketType::C2S_EnterChannel:
		ReqEnterChannel();
		break;

	case eTcpPacketType::C2S_EnterLobby:
		ReqEnterLobby();
		break;

	case eTcpPacketType::C2S_CreateRoom:
		ReqCreateRoom(packetTemp);
		break;

	case eTcpPacketType::C2S_EnterRoom:
		ReqEnterRoom(packetTemp);
		break;

	case eTcpPacketType::C2S_QuickEnterRoom:
		ReqQuickEnterRoom();
		break;

	case eTcpPacketType::C2S_LeaveRoom:
		ReqLeaveRoom();
		break;

	case eTcpPacketType::C2S_ChangeReady:
		ReqChangeReady();
		break;

	case eTcpPacketType::C2S_ChangeCharacterType:
		ReqChangeCharacterType(packetTemp);
		break;

	case eTcpPacketType::C2S_ChatLobby:
		ResChatLobby(packetTemp);
		break;

	case eTcpPacketType::C2S_ChatRoom:
		ResChatRoom(packetTemp);
		break;

	case eTcpPacketType::C2S_RoomProfileAndUsersInfo:
		ResRoomInfo();
		break;

	case eTcpPacketType::C2S_UserPage:
		ReqUserPage(packetTemp);
		break;

	case eTcpPacketType::C2S_RoomPage:
		ReqRoomPage(packetTemp);
		break;

	case eTcpPacketType::C2S_GameStart:
		ReqGameStart();
		break;

	case eTcpPacketType::C2S2C_GameState:
		m_room->EndGame(_packet);
		break;

	case eTcpPacketType::C2S_GameData:
		ReqGameData();
		break;

	case eTcpPacketType::C2S_UDPAddressInfo:
		ReqUDPInfo(packetTemp);
		break;

	default:
		break;
	}
}

void CUser::ReqCreateName(byte* _data)
{
	byte* dataTemp = _data;

	m_profile.nameBuffLength = CSmartBuffer::ReadByte(&dataTemp);
	memcpy(m_profile.nameBuff, dataTemp, m_profile.nameBuffLength);

	CUserManager* userManager = CServerResourceProvider::GetInstance()->GetChannel()->GetUserManager();
	bool duplicate = userManager->IsDuplicateName(this, m_profile.nameBuffLength, m_profile.nameBuff);

	if (duplicate)	ResCreateNameResult(false);
	else			ResCreateNameResult(true);
}

void CUser::ReqEnterChannel()
{
	CLobby* lobby = CServerResourceProvider::GetInstance()->GetChannel()->Enter(this);

	if (lobby)	{ m_lobby = lobby; ResEnterChannelResult(true); }
	else		{ ResEnterChannelResult(false); }
}

void CUser::ReqEnterLobby()
{
	if (m_lobby->Enter(this))	ResEnterLobbyResult(true);
	else						ResEnterLobbyResult(false);
}

void CUser::ReqCreateRoom(byte* _data)
{
	byte* dataTemp = _data;
	byte nameSize = CSmartBuffer::ReadByte(&dataTemp);

	CRoom* room = m_lobby->GetInactRoom();
	if (!room) ResCreateRoomResult(false);
	else
	{
		m_lobby->Leave(this);
		m_room = room;
		m_room->Open(this, dataTemp, nameSize);
		ResCreateRoomResult(true);
	}
}

void CUser::ReqEnterRoom(byte* _data)
{
	byte* dataTemp = _data;
	u_short roomNum = CSmartBuffer::ReadWord(&dataTemp);
	
	CRoom* room = m_lobby->GetRoom(roomNum);
	if (!room) ResEnterRoomResult(false);
	else
	{
		byte myIndex = 0;
		if(!room->Enter(this)) ResEnterRoomResult(false);
		else
		{
			m_room = room;
			m_lobby->Leave(this);
			ResEnterRoomResult(true);
		}
	}
}

void CUser::ReqQuickEnterRoom()
{
	CRoom* room = m_lobby->GetRoom();
	if (!room) ResEnterRoomResult(false);
	else
	{
		byte myIndex = 0;
		if (!room->Enter(this)) ResEnterRoomResult(false);
		else
		{
			m_room = room;
			m_lobby->Leave(this);
			ResEnterRoomResult(true);
		}
	}
}

void CUser::ReqLeaveRoom()
{
	m_room->Leave(this);
	m_room = nullptr;
	ResLeaveRoom();
}

void CUser::ReqChangeReady()
{
	m_room->ChangeReady(this);
}

void CUser::ReqChangeCharacterType(byte* _data)
{
	m_room->ChangeChracterType(this, *reinterpret_cast<eCharacterType*>(_data));
}

void CUser::ReqUserPage(byte* _data)
{
	byte* dataTemp = _data;
	u_short requestedPageIndex = CSmartBuffer::ReadWord(&dataTemp);

	CPage page(0, 0);
	u_short lastPageIndex = m_lobby->GetUserPage(requestedPageIndex, page);

	ResUserPage(page, lastPageIndex);
}

void CUser::ReqRoomPage(byte* _data)
{
	byte* dataTemp = _data;
	u_short requestedPageIndex = CSmartBuffer::ReadWord(&dataTemp);

	CPage page(0, 0);
	u_short lastPageIndex = m_lobby->GetRoomPage(requestedPageIndex, page);

	ResRoomPage(page, lastPageIndex);
}

void CUser::ReqGameStart()
{
	m_room->StartGame();
}

void CUser::ReqGameData()
{
	byte index;
	eRole role;
	std::vector<PLAYER_DATA> list;
	m_room->GetGameData(this, index, role, list);
	ResGameData(index, role, list);
}

void CUser::ReqUDPInfo(byte* _data)
{
	UDPADDRESS_INFO info;

	info.sourceIp = CSmartBuffer::ReadULONG(&_data);
	info.sourcePort = CSmartBuffer::ReadWord(&_data);
	info.destinationIp = CSmartBuffer::ReadULONG(&_data);
	info.destinationPort = CSmartBuffer::ReadWord(&_data);

	m_room->SetUDPAddr(this, info);
}

void CUser::ResCreateNameResult(bool _result)
{
	m_sendBuffStream.Reset();
	m_sendBuffStream.Write(static_cast<u_short>(g_headerSize + sizeof(byte)));
	m_sendBuffStream.Write(static_cast<u_short>(eTcpPacketType::S2C_CreateNameResult));
	m_sendBuffStream.Write(static_cast<byte>(_result));

	SendPacket(m_sendBuffStream.GetBuff());
}

void CUser::ResEnterChannelResult(bool _result)
{
	m_sendBuffStream.Reset();
	m_sendBuffStream.Write(static_cast<u_short>(g_headerSize + sizeof(byte)));
	m_sendBuffStream.Write(static_cast<u_short>(eTcpPacketType::S2C_EnterChannelResult));
	m_sendBuffStream.Write(static_cast<byte>(_result));

	SendPacket(m_sendBuffStream.GetBuff());
}

void CUser::ResEnterLobbyResult(bool _result)
{
	m_sendBuffStream.Reset();
	m_sendBuffStream.Write(static_cast<u_short>(g_headerSize + sizeof(byte)));
	m_sendBuffStream.Write(static_cast<u_short>(eTcpPacketType::S2C_EnterLobbyResult));
	m_sendBuffStream.Write(static_cast<byte>(_result)); 

	SendPacket(m_sendBuffStream.GetBuff());
}

void CUser::ResCreateRoomResult(bool _result)
{
	m_sendBuffStream.Reset();
	m_sendBuffStream.Write(static_cast<u_short>(g_headerSize + sizeof(byte)));
	m_sendBuffStream.Write(static_cast<u_short>(eTcpPacketType::S2C_CreateRoomResult));
	m_sendBuffStream.Write(static_cast<byte>(_result));

	SendPacket(m_sendBuffStream.GetBuff());
}

void CUser::ResEnterRoomResult(bool _result)
{
	m_sendBuffStream.Reset();
	m_sendBuffStream.Write(static_cast<u_short>(0));
	m_sendBuffStream.Write(static_cast<u_short>(eTcpPacketType::S2C_EnterRoomResult));
	m_sendBuffStream.Write(static_cast<byte>(_result));

	m_sendBuffStream.WriteSizeToFront();

	SendPacket(m_sendBuffStream.GetBuff());
}

void CUser::ResLeaveRoom()
{
	m_sendBuffStream.Reset();
	m_sendBuffStream.Write(static_cast<u_short>(g_headerSize));
	m_sendBuffStream.Write(static_cast<u_short>(eTcpPacketType::S2C_LeaveRoom));

	SendPacket(m_sendBuffStream.GetBuff());
}

void CUser::ResIsReady()
{
	m_sendBuffStream.Reset();
	m_sendBuffStream.Write(static_cast<u_short>(g_headerSize));
	m_sendBuffStream.Write(static_cast<u_short>(eTcpPacketType::S2C_IsReady));

	SendPacket(m_sendBuffStream.GetBuff());
}

void CUser::ResChatLobby(byte* _data)
{
	byte* dataTemp = _data;
	u_short chatSize = CSmartBuffer::ReadWord(&dataTemp);

	m_sendBuffStream.Reset();
	m_sendBuffStream.Write(static_cast<u_short>(0));
	m_sendBuffStream.Write(static_cast<u_short>(eTcpPacketType::S2C_ChatLobby));
	m_sendBuffStream.Write(static_cast<u_short>(m_profile.nameBuffLength + 6 + chatSize));
	m_sendBuffStream.Write(m_profile.nameBuff, m_profile.nameBuffLength);
	m_sendBuffStream.Write(L" : ", 6);
	m_sendBuffStream.Write(dataTemp, chatSize);
	m_sendBuffStream.WriteSizeToFront();

	m_lobby->SendPacketToAll(m_sendBuffStream.GetBuff());
}

void CUser::ResChatRoom(byte* _data)
{
	byte* dataTemp = _data;
	u_short chatSize = CSmartBuffer::ReadWord(&dataTemp);

	m_sendBuffStream.Reset();
	m_sendBuffStream.Write(static_cast<u_short>(0));
	m_sendBuffStream.Write(static_cast<u_short>(eTcpPacketType::S2C_ChatRoom));
	m_sendBuffStream.Write(static_cast<u_short>(m_profile.nameBuffLength + 6 + chatSize));
	m_sendBuffStream.Write(m_profile.nameBuff, m_profile.nameBuffLength);
	m_sendBuffStream.Write(L" : ", 6);
	m_sendBuffStream.Write(dataTemp, chatSize);
	m_sendBuffStream.WriteSizeToFront();

	m_room->SendPacketToAll(m_sendBuffStream.GetBuff());
}

void CUser::ResRoomInfo()
{
	m_sendBuffStream.Reset();
	m_sendBuffStream.Write(static_cast<u_short>(0));
	m_sendBuffStream.Write(static_cast<u_short>(eTcpPacketType::S2C_RoomProfileAndUsersInfo));
	m_room->WriteRoomInfo(this, m_sendBuffStream);
	m_sendBuffStream.WriteSizeToFront();

	SendPacket(m_sendBuffStream.GetBuff());
}

void CUser::ResUserPage(CPage& _page, u_short _lastPageIndex)
{
	u_short responsePageIndex = _page.GetIndex();
	CMemoryStream& pageStream = _page.GetPageStream();

	m_sendBuffStream.Reset();
	m_sendBuffStream.Write(static_cast<u_short>(0));
	m_sendBuffStream.Write(static_cast<u_short>(eTcpPacketType::S2C_UserPage));
	m_sendBuffStream.Write(_lastPageIndex);
	m_sendBuffStream.Write(responsePageIndex);
	m_sendBuffStream.Write(pageStream.GetBuff(), pageStream.GetSize());
	m_sendBuffStream.WriteSizeToFront();

	SendPacket(m_sendBuffStream.GetBuff());
}

void CUser::ResRoomPage(CPage& _page, u_short _lastPageIndex)
{
	u_short responsePageIndex = _page.GetIndex();
	CMemoryStream& pageStream = _page.GetPageStream();

	m_sendBuffStream.Reset();
	m_sendBuffStream.Write(static_cast<u_short>(0));
	m_sendBuffStream.Write(static_cast<u_short>(eTcpPacketType::S2C_RoomPage));
	m_sendBuffStream.Write(_lastPageIndex);
	m_sendBuffStream.Write(responsePageIndex);
	m_sendBuffStream.Write(pageStream.GetBuff(), pageStream.GetSize());
	m_sendBuffStream.WriteSizeToFront();

	SendPacket(m_sendBuffStream.GetBuff());
}

void CUser::ResGameData(byte _index, eRole _role, std::vector<PLAYER_DATA>& _list)
{
	int size = _list.size();

	m_sendBuffStream.Reset();
	m_sendBuffStream.Write(static_cast<u_short>(0));
	m_sendBuffStream.Write(static_cast<u_short>(eTcpPacketType::S2C_GameData));
	m_sendBuffStream.Write(static_cast<byte>(size));
	m_sendBuffStream.Write(_index);
	m_sendBuffStream.Write(static_cast<byte>(_role));

	for (int i = 0; i < size; ++i)
	{
		PLAYER_DATA& temp = _list[i];
		m_sendBuffStream.Write(temp.index);
		m_sendBuffStream.Write(temp.nameBuffLength);
		m_sendBuffStream.Write(temp.nameBuff, temp.nameBuffLength);
		m_sendBuffStream.Write(static_cast<byte>(temp.role));
		m_sendBuffStream.Write(static_cast<byte>(temp.charType));
		m_sendBuffStream.Write(static_cast<byte>(temp.color));
	}
	m_sendBuffStream.WriteSizeToFront();

	SendPacket(m_sendBuffStream.GetBuff());
}