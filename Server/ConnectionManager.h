#pragma once
#include "Connection.h"
#include <set>
class CConnectionManager
{
private:
	CRITICAL_SECTION m_cs;
	std::set<CConnection*> m_connectionList;
	unsigned int m_capacity;

public:
	CConnectionManager(unsigned int _capacity);
	~CConnectionManager();

	bool Add(CConnection* _pConnection);
	void Remove(CConnection* _pConnection);

	void SendAll(byte* _packet);
};