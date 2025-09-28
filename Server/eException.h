#pragma once
enum class eException : short
{
	Fail_WSAStartup = 0,
	Fail_CreateIpCompletionPort,
	Fail_socketFunc,
	Fail_inet_pton,
	Fail_UDPBind,
	Error_QueueOverFlow,
	Error_QueueUnderFlow,
};