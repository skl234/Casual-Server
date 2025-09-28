#include "ConnectionManager.h"

CConnectionManager::CConnectionManager(unsigned int _capacity)
	:m_capacity(_capacity)
{
	InitializeCriticalSection(&m_cs);
}

CConnectionManager::~CConnectionManager()
{
	std::set<CConnection*>::iterator iter = m_connectionList.begin();
	std::set<CConnection*>::iterator end = m_connectionList.end();
	for (iter; iter != end; ++iter)
	{
		delete iter._Ptr->_Myval;
	}
	DeleteCriticalSection(&m_cs);
}

bool CConnectionManager::Add(CConnection* _pConnection)
{
	bool result = true;

	EnterCriticalSection(&m_cs);
	if (m_capacity <= m_connectionList.size()) result = false;
	else m_connectionList.insert(_pConnection);
	LeaveCriticalSection(&m_cs);

	return result;
}

void CConnectionManager::Remove(CConnection* _pConnection)
{
	EnterCriticalSection(&m_cs);
	m_connectionList.erase(_pConnection);
	delete _pConnection;
	LeaveCriticalSection(&m_cs);
}

void CConnectionManager::SendAll(byte* _packet)
{
	EnterCriticalSection(&m_cs);
	std::set<CConnection*>::iterator iter = m_connectionList.begin();
	std::set<CConnection*>::iterator end = m_connectionList.end();
	for (iter; iter != end; ++iter)
	{
		(*iter)->Send(_packet, *reinterpret_cast<WORD*>(_packet));
	}
	LeaveCriticalSection(&m_cs);
}