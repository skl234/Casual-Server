#include "Channel.h"
CChannel::CChannel(u_short _capacity)
{
	m_userManager = new CUserManager(_capacity);
	m_lobby = new CLobby(_capacity, m_userManager);
}

CChannel::~CChannel()
{
	if (m_lobby) { delete m_lobby; m_lobby = nullptr; }
	if (m_userManager) { delete m_userManager; m_userManager = nullptr; }
}

CLobby* CChannel::Enter(CUser* _user)
{
	if (!m_userManager->Add(_user)) return nullptr;
	return m_lobby;
}

void CChannel::Leave(CUser* _user)
{
	m_userManager->Erase(_user);
}

CUserManager* CChannel::GetUserManager()
{
	return m_userManager;
}