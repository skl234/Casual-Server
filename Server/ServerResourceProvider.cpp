#include "ServerResourceProvider.h"

CServerResourceProvider::CServerResourceProvider()
	:m_inServerUserManager(nullptr),
	m_channel(nullptr)
{

}

CServerResourceProvider::~CServerResourceProvider()
{

}

CServerResourceProvider* CServerResourceProvider::GetInstance()
{
	static CServerResourceProvider instance;
	return &instance;
}

void CServerResourceProvider::Register(CUserManager* _inServerUserManager, CChannel* _channel)
{
	m_inServerUserManager = _inServerUserManager;
	m_channel = _channel;
}

CUserManager* CServerResourceProvider::GetServerUserManager()
{
	return m_inServerUserManager;
}

CChannel* CServerResourceProvider::GetChannel()
{
	return m_channel;
}