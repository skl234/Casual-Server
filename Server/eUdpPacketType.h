#pragma once
typedef unsigned short u_short;
enum class eUdpPacketType : u_short
{
	Begin = 0,

	C2S_Hello,
	C2S_KeepMapping,

	S2C_HelloResponse,

	C2C_Hello,
	C2C_HelloResponse,
	C2C_KeepMapping,

	End
};