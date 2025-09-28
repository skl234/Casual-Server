#include "UserPageManager.h"

constexpr byte g_sizeOfPerPage = 12;
constexpr u_short g_pageBuffSize = (sizeof(USER_PROFILE) * g_sizeOfPerPage) + sizeof(u_short);

CUserPageManager::CUserPageManager(u_short _capacity, CUserManager* _userManager)
	:CPageManager(_capacity),
	m_userManager(_userManager)
{
	size_t size = m_pageList.size();
	for (size_t i = 0; i < size; ++i)
		m_pageList[i] = new CPage(i, g_pageBuffSize);

	m_defaultPage = new CPage(0, g_pageBuffSize);
	m_defaultPage->Write(static_cast<u_short>(0));
}

CUserPageManager::~CUserPageManager()
{
	size_t size = m_pageList.size();
	for (size_t i = 0; i < size; ++i)
		if (m_pageList[i]) delete m_pageList[i];

	delete m_defaultPage;
}

bool CUserPageManager::Update(u_short& _index)
{
	std::vector<USER_PROFILE> profileList;
	profileList.reserve(g_sizeOfPerPage);

	if (!m_userManager->GetProfilesAndLastPageIndex(
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
		USER_PROFILE& userProfile = profileList[i];
		page->Write(userProfile.nameBuffLength);
		page->Write(userProfile.nameBuff, userProfile.nameBuffLength);
	}
	m_timerList[_index].Reset();
	return true;
}