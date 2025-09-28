#pragma once
#include <WinSock2.h>
struct WSAINFO
{
	WSAOVERLAPPED overlapped;
	WSABUF wsaBuf;
	DWORD flags;
};