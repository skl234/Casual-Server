#pragma once
#include "PageManager.h"
#include "UserManager.h"
class CUserPageManager : public CPageManager
{
private:
	CUserManager* m_userManager;

public:
	CUserPageManager(u_short _capacity, CUserManager* _userManager);
	~CUserPageManager() override;

protected:
	bool Update(u_short& _index) override;
};