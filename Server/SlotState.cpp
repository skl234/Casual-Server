#include "SlotState.h"
#include "User.h"
CSlotState::CSlotState()
{
	m_user = nullptr;
	m_ready = false;
	m_role = eRole::GUEST;
	m_charType = eCharacterType::MouseMan;
}

CSlotState::~CSlotState()
{

}

void CSlotState::Clear()
{
	m_user = nullptr;
	m_ready = false;
	m_role = eRole::GUEST;
	m_charType = eCharacterType::MouseMan;
}

bool CSlotState::IsEmpty()
{
	if (m_user) return false;

	return true;
}

void CSlotState::SetUser(CUser* _user, eRole _role)
{
	m_user = _user;
	m_role = _role;
}

bool CSlotState::ChangeReady()
{
	if (m_ready) m_ready = false;
	else m_ready = true;

	return m_ready;
}

void CSlotState::UnReady()
{
	m_ready = false;
}

void CSlotState::SetRole(eRole _role)
{
	m_role = _role;
	if (m_role == eRole::HOST) m_ready = false;
}

void CSlotState::SetCharacterType(eCharacterType _type)
{
	m_charType = _type;
}

CUser* CSlotState::GetUser()
{
	return m_user;
}

bool CSlotState::GetReady()
{
	return m_ready;
}

eRole CSlotState::GetRole()
{
	return m_role;
}

eCharacterType CSlotState::GetCharType()
{
	return m_charType;
}

void CSlotState::GetData(CMemoryStream& _stream)
{
	USER_PROFILE profile;
	m_user->GetProfile(profile);
	_stream.Write(profile.nameBuffLength);
	_stream.Write(profile.nameBuff, profile.nameBuffLength);
	_stream.Write(static_cast<byte>(m_ready));
	_stream.Write(static_cast<byte>(m_role));
	_stream.Write(static_cast<byte>(m_charType));
}