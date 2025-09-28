#pragma once
#include "SlotState.h"
#include "USER_PROFILE.h"
enum class eTeamColor : byte
{
	Red = 0,
	Blue,
};

struct PLAYER_DATA
{
public:
	byte index;
	byte nameBuffLength = 0;
	byte nameBuff[g_maxSizeUserNameBuff];
	eRole role;
	eCharacterType charType;
	eTeamColor color;
};