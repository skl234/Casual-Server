#include "UserManager.h"
#include <vector>
#include "eTcpPacketType.h"

CUserManager::CUserManager(size_t _capacity)
	:m_capacity(_capacity)
{
	InitializeCriticalSection(&m_cs);
}

CUserManager::~CUserManager()
{
	DeleteCriticalSection(&m_cs);
}

bool CUserManager::Add(CUser* _user)
{
	EnterCriticalSection(&m_cs);
	size_t size = m_userList.size();
	if (size >= m_capacity)
	{
		LeaveCriticalSection(&m_cs);
		return false;
	}
	m_userList.insert(_user);
	LeaveCriticalSection(&m_cs);
	return true;
}

void CUserManager::Erase(CUser* _user)
{
	EnterCriticalSection(&m_cs);
	m_userList.erase(_user);
	LeaveCriticalSection(&m_cs);
}

void CUserManager::ReleaseAll()
{
	EnterCriticalSection(&m_cs);
	std::set<CUser*>::iterator iter = m_userList.begin();
	std::set<CUser*>::iterator end = m_userList.end();
	for (iter; iter != end; ++iter)
	{
		delete iter._Ptr->_Myval;
	}
	m_userList.clear();
	LeaveCriticalSection(&m_cs);
}

void CUserManager::SendPacketToAll(byte* _packet)
{
	EnterCriticalSection(&m_cs);
	std::set<CUser*>::iterator iter = m_userList.begin();
	std::set<CUser*>::iterator end = m_userList.end();
	for (iter; iter != end; ++iter)
	{
		(*iter)->SendPacket(_packet);
	}
	LeaveCriticalSection(&m_cs);
}

bool CUserManager::IsDuplicateName(CUser* _user, byte _length, byte* _nameBuff)
{
	bool result = false;
	EnterCriticalSection(&m_cs);
	std::set<CUser*>::iterator iter = m_userList.begin();
	std::set<CUser*>::iterator end = m_userList.end();
	USER_PROFILE userProfile;
	for (iter; iter != end; ++iter)
	{
		if (_user == *iter) continue;
		(*iter)->GetProfile(userProfile);
		if (userProfile.nameBuffLength != _length) continue;
		if (!std::equal(userProfile.nameBuff, userProfile.nameBuff + userProfile.nameBuffLength, _nameBuff)) continue;
		
		result = true;
		break;
	}
	LeaveCriticalSection(&m_cs);

	return result;
}

bool CUserManager::GetProfilesAndLastPageIndex(u_short& _pageIndex,
	std::vector<USER_PROFILE>& _list, u_short& _lastPageIndex)
{
	EnterCriticalSection(&m_cs);
	size_t userListSize = m_userList.size();
	if (userListSize == 0)
	{
		LeaveCriticalSection(&m_cs);
		return false;
	}

	std::set<CUser*>::iterator iter = m_userList.begin();
	std::set<CUser*>::iterator end = m_userList.end();

	//pageIndex 할당
	size_t profileCapacity = _list.capacity();
	_lastPageIndex = (userListSize - 1) / profileCapacity;
	if (_pageIndex > _lastPageIndex)
	{
		_pageIndex = _lastPageIndex;
	}

	for (u_int i = 0; i < _pageIndex * profileCapacity; ++i)
	{
		++iter;
	}
	//Profile 할당
	USER_PROFILE profile;
	for (iter; iter != end; ++iter)
	{
		if (_list.size() >= profileCapacity) break;

		(*iter)->GetProfile(profile);
		if (profile.nameBuffLength == 0) continue;
		_list.push_back(profile);
	}

	LeaveCriticalSection(&m_cs);

	return true;
}