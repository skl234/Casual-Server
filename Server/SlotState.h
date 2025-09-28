#pragma once
#include "eRole.h"
#include "eCharacterType.h"
#include <vector>
#include "MemoryStream.h"

class CUser;

class CSlotState
{
private:
	CUser* m_user;
	bool m_ready;
	eRole m_role;
	eCharacterType m_charType;

public:
	CSlotState();
	~CSlotState();

	void Clear();
	bool IsEmpty();
	void SetUser(CUser* _user, eRole _role);
	bool ChangeReady();
	void UnReady();
	void SetRole(eRole _role);
	void SetCharacterType(eCharacterType _type);

	CUser* GetUser();
	bool GetReady();
	eRole GetRole();
	eCharacterType GetCharType();

	void GetData(CMemoryStream& _stream);
};