#pragma once
typedef unsigned char byte;
typedef unsigned short u_short;
constexpr byte g_maxSizeRoomNameBuff = 20;

enum class eRoomState : byte
{
	WAITING = 0,
	PLAYING,
};

struct ROOM_PROFILE
{
	u_short number = 0;
	byte nameBuffLength = 0;
	byte nameBuff[g_maxSizeRoomNameBuff] = {};
	eRoomState state = eRoomState::WAITING;
	byte userSize = 0;
	byte userMax = 0;
};

