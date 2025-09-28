#include "WSARecvRoutine.h"
CWSARecvRoutine::CWSARecvRoutine(CIOCP* _pIOCP, CConnectionManager* _pConnectionManager)
	:CWorkerRoutine(_pIOCP),
	m_pConnectionManager(_pConnectionManager)
{

}

CWSARecvRoutine::~CWSARecvRoutine()
{

}

bool CWSARecvRoutine::StopRoutine(DWORD _error, void* _completionKey)
{
	if (_error == ERROR_NETNAME_DELETED)
	{
		m_pConnectionManager->Remove(reinterpret_cast<CConnection*>(_completionKey));
		return false;
	}
	else if (_error == ERROR_CONNECTION_ABORTED)
	{
		return false;
	}

	printf("%d\n", _error);

	return true;
}

void CWSARecvRoutine::OnIOCompletion(DWORD _byteTrans, void* _completionKey)
{
	CConnection* keyTemp = reinterpret_cast<CConnection*>(_completionKey);

	if (_byteTrans == 0)
	{
		m_pConnectionManager->Remove(keyTemp);
		return;
	}

	if (!keyTemp->OnRecv(_byteTrans))
	{
		m_pConnectionManager->Remove(keyTemp);
		return;
	}

	if (!keyTemp->AsyncRecv())
	{
		m_pConnectionManager->Remove(keyTemp);
		return;
	}
}