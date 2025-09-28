#pragma once
class CServerApp
{
public:
	CServerApp();
	virtual ~CServerApp();

	void Initialize(const char* _addr, int _tcpPort, int _udpPort);
	void Cleanup();

	virtual void Run() = 0;

protected:
	virtual void OnInit(const char* _addr, int _tcpPort, int _udpPort) = 0;
	virtual void OnCleanup() = 0;
};