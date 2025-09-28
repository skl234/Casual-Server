#include "IOCPServerApp.h"
#include <stdio.h>
#include <locale.h>
#include "eException.h"

#pragma comment(lib, "Ws2_32.lib")

constexpr char g_serverAddr[] = "125.186.182.181";
constexpr u_short g_tcpPort = 30002;
constexpr u_short g_udpPort = 30003;

CIOCPServerApp* g_app = nullptr;

int WINAPI CtrlHandler(DWORD ctrlType)
{
	//ctrl+c 또는 x버튼을 눌러서 종료할때
	if (ctrlType == CTRL_C_EVENT || ctrlType == CTRL_CLOSE_EVENT)
	{
		printf("종료\n");
		if (g_app)
		{
			g_app->Cleanup();
			delete g_app;
			g_app = nullptr;
		}
		return true;
	}
	return false;
}

int main()
{
	setlocale(LC_ALL, "korean");
	_wsetlocale(LC_ALL, L"korean");

	SetConsoleCtrlHandler(CtrlHandler, TRUE);

	g_app = new CIOCPServerApp();
	try
	{
		g_app->Initialize(g_serverAddr, g_tcpPort, g_udpPort);
		g_app->Run();
	}
	catch (eException& _ec)
	{
		printf("eException : %d\n", (int)_ec);
		printf("Error Code : %d\n", WSAGetLastError());
	}

	if (g_app)
	{
		g_app->Cleanup();
		delete g_app;
		g_app = nullptr;
	}
}