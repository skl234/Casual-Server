#pragma once
#include "Channel.h"

class CServerResourceProvider
{
private:
	CUserManager* m_inServerUserManager;
	CChannel* m_channel;

private:
	CServerResourceProvider();
	~CServerResourceProvider();
	
public:
	static CServerResourceProvider* GetInstance();

	void Register(CUserManager* _inServerUserManager, CChannel* _channel);
	CUserManager* GetServerUserManager();
	CChannel* GetChannel();
};