#pragma once
#include <WinSock2.h>
struct UDPADDRESS_INFO
{
public:
	ULONG	sourceIp;			
	u_short sourcePort;			
	ULONG	destinationIp;		
	u_short destinationPort;	
};