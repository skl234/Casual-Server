#include "RoomPageManager.h"

constexpr byte g_sizeOfPerPage = 6;
constexpr u_short g_pageBuffSize = (sizeof(ROOM_PROFILE) * g_sizeOfPerPage) + +sizeof(u_short);

CRoomPageManager::CRoomPageManager(u_short _capacity, CRoomManager* _roomManager)
	:CPageManager(_capacity),
	m_roomManager(_roomManager)
{
	size_t size = m_pageList.size();
	for (size_t i = 0; i < size; ++i)
		m_pageList[i] = new CPage(i, g_pageBuffSize);

	m_defaultPage = new CPage(0, g_pageBuffSize);
	m_defaultPage->Write(static_cast<u_short>(0));
}

CRoomPageManager::~CRoomPageManager()
{
	size_t size = m_pageList.size();
	for (size_t i = 0; i < size; ++i)
		if (m_pageList[i]) delete m_pageList[i];

	delete m_defaultPage;
}

bool CRoomPageManager::Update(u_short& _index)
{
	std::vector<ROOM_PROFILE> profileList;
	profileList.reserve(g_sizeOfPerPage);

	if (!m_roomManager->GetActRoomManager()->GetProfilesAndLastPageIndex(
		_index, profileList, m_lastPageIndex))
	{
		return false;
	}

	size_t size = profileList.size();
	CPage* page = m_pageList[_index];
	page->Reset();
	page->Write(static_cast<u_short>(size));
	for (size_t i = 0; i < size; ++i)
	{
		ROOM_PROFILE& roomProfile = profileList[i];

		page->Write(roomProfile.number);
		page->Write(roomProfile.nameBuffLength);
		page->Write(roomProfile.nameBuff, roomProfile.nameBuffLength);
		page->Write(static_cast<byte>(roomProfile.state));
		page->Write(roomProfile.userSize);
		page->Write(roomProfile.userMax);
	}
	m_timerList[_index].Reset();
	return true;
}