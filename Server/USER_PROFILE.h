#pragma once
typedef unsigned char byte;
constexpr byte g_maxSizeUserNameBuff = 20;

// \0 �� ���Ѱ� ������ ������ʿ� x
// Ŭ���̾�Ʈ���� ó��
struct USER_PROFILE
{
	byte nameBuffLength = 0;
	byte nameBuff[g_maxSizeUserNameBuff];
};