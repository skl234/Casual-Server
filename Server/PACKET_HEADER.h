#pragma once
#define HEADER_SIZE 4

enum class ePacket_Type : unsigned short
{
	Begin = 0,

	//Server_to_Client,
	Full_Server,
	Success_Enter_Server,
	Success_Enter_Lobby,
	Fail_Enter_Lobby,

	//Client_to_Server,
	Request_Enter_Lobby,
	Request_UserList,

	End
};

struct PACKET_HEADER
{
	unsigned short m_packetSize;
	ePacket_Type m_packetType;
};