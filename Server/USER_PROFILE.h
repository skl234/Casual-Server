#pragma once
typedef unsigned char byte;
constexpr byte g_maxSizeUserNameBuff = 20;

// \0 에 관한건 서버가 고려할필요 x
// 클라이언트에서 처리
struct USER_PROFILE
{
	byte nameBuffLength = 0;
	byte nameBuff[g_maxSizeUserNameBuff];
};