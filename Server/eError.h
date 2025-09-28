#pragma once
enum class eError : short
{
	Fail_socket = 0,
	Fail_inet_pton,
	Fail_WSAStartup,
	Fail_CreateThread,
	Fail_listenStart,
	Fail_listenAccept,
	Fail_CreateIpCompletionPort,

};