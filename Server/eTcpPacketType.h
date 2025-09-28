#pragma once
typedef unsigned unsigned char byte;
typedef unsigned short u_short;

constexpr byte g_headerSize = 4;
constexpr byte g_headerFrontSize = 2;
constexpr byte g_headerBackSize = 2;

enum class eTcpPacketType : u_short
{
	Begin = 0,

	//Server_to_Client,
	S2C_EnterServerResult,
	S2C_CreateNameResult,
	S2C_EnterChannelResult,
	S2C_EnterLobbyResult,
	S2C_UserPage,
	S2C_RoomPage,
	S2C_ChatLobby,
	S2C_CreateRoomResult,
	S2C_EnterRoomResult,
	S2C_RoomProfileAndUsersInfo,
	S2C_ChatRoom,
	S2C_EnteredRoomUser,
	S2C_LeavedRoomUser,
	S2C_ChangeReady,
	S2C_NewHostIndex,
	S2C_LeaveRoom,
	S2C_GameStart,
	S2C_GameData,
	S2C_UDPAddressInfo,
	S2C_CompleteMapped,
	S2C_WaitForGameStart,
	S2C_ChangeCharacterType,
	S2C_IsReady,

	//Client_to_Server
	C2S_CreateName,
	C2S_EnterChannel,
	C2S_EnterLobby,
	C2S_UserPage,
	C2S_RoomPage,
	C2S_ChatLobby,
	C2S_CreateRoom,
	C2S_EnterRoom,
	C2S_QuickEnterRoom,
	C2S_RoomProfileAndUsersInfo,
	C2S_LeaveRoom,
	C2S_ChatRoom,
	C2S_ChangeReady,
	C2S_GameStart,
	C2S_GameData,
	C2S_UDPAddressInfo,
	C2S_CompleteMapped,
	C2S_WaitForGameStart,
	C2S_ChangeCharacterType,

	//EndGame
	C2S2C_GameState,

	End
};