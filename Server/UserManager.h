#pragma once
#include <WinSock2.h>
#include <set>
#include <vector>
#include "User.h"
#include "Page.h"

class CUserManager
{
private:
	CRITICAL_SECTION m_cs;
	std::set<CUser*> m_userList;
	size_t m_capacity;

public:
	CUserManager(size_t _capacity);
	~CUserManager();

	bool Add(CUser* _user);
	void Erase(CUser* _user);
	void ReleaseAll();
	void SendPacketToAll(byte* _packet);

	bool IsDuplicateName(CUser* _user, byte _length, byte* _nameBuff);
	//return fail : 유저 수 0명
	bool GetProfilesAndLastPageIndex(u_short& _pageIndex,
		std::vector<USER_PROFILE>& _list, u_short& _lastPageIndex);

};