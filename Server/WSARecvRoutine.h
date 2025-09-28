#pragma once
#include "WorkerRoutine.h"
#include "ConnectionManager.h"
class CWSARecvRoutine : public CWorkerRoutine
{
private:
	CConnectionManager* m_pConnectionManager;

public:
	CWSARecvRoutine(CIOCP* _pIOCP, CConnectionManager* _pConnectionManager);
	virtual ~CWSARecvRoutine();

private:
	virtual bool StopRoutine(DWORD _error, void* _completionKey);
	virtual void OnIOCompletion(DWORD _byteTrans, void* _completionKey);
};

//4Hand Shake 는 일단 보류