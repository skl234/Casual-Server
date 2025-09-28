#pragma once
#include "PageManager.h"
#include "RoomManager.h"
class CRoomPageManager : public CPageManager
{
private:
	CRoomManager* m_roomManager;

public:
	CRoomPageManager(u_short _capacity, CRoomManager* _roomManager);
	~CRoomPageManager() override;

protected:
	bool Update(u_short& _index) override;
};