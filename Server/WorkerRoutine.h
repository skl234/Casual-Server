#pragma once
#include "Thread.h"
#include "IOCP.h"

class CWorkerRoutine : public CThread
{
protected:
	CIOCP* m_pIOCP;

public:
	CWorkerRoutine(CIOCP* _pIOCP);
	virtual ~CWorkerRoutine();

protected:
	virtual void Run();
	virtual bool StopRoutine(DWORD _error, void* _completionKey) = 0;
	virtual void OnIOCompletion(DWORD _byteTrans, void* _completionKey) = 0;
};