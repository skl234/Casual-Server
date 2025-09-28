#include "IOCPServerApp.h"
#include "WorkerThread.h"
#include "eException.h"
#include "eTcpPacketType.h"
#include "MemoryStream.h"
#include "ServerResourceProvider.h"

constexpr u_short g_maxUser = 300;

CIOCPServerApp::CIOCPServerApp()
	:m_listener(nullptr),
	m_iocp(nullptr),
	m_inServerUserManager(nullptr),
	m_channel(nullptr),
	m_udpSocket(nullptr),
	m_udpThread(nullptr)
{
	
}

CIOCPServerApp::~CIOCPServerApp()
{
	
}

void CIOCPServerApp::Run()
{
	if (!m_listener->Start())
	{
		printf("Listen Start Error : %d\n", WSAGetLastError());
		return;
	}

	m_udpThread->Start();

	SOCKET		socket;
	SOCKADDR_IN	addr;
	CUser*		user;

	while (true)
	{
		if (!m_listener->Accept(socket, addr))
		{	
			int error = WSAGetLastError();
			if (error == WSAEINTR) printf("Closed Listen Socket, Listen Break \n");
			else printf("Listen Socket Error : %d\n", error);

			break;
		}

		user = new CUser(socket, addr);

		if (m_inServerUserManager->Add(user)) ResSuccessEnteredServer(user);
		else ResFailEnteredServer(user);
	}
}

void CIOCPServerApp::OnInit(const char* _addr, int _tcpPort, int _udpPort)
{
	m_inServerUserManager =	new CUserManager(g_maxUser);
	m_channel =				new CChannel(g_maxUser);

	CServerResourceProvider* srp = CServerResourceProvider::GetInstance();
	srp->Register(m_inServerUserManager, m_channel);

	if (!m_listener) m_listener = new CTCPListener(_addr, _tcpPort);
	if (!m_iocp) m_iocp = new CIOCP();

	m_udpSocket = new CUDPSocket();
	if (SOCKET_ERROR == m_udpSocket->Bind(_addr, _udpPort))
	{
		printf("UDP Bind Error\n");
		throw eException::Fail_UDPBind;
	}
	m_udpThread = new CUDPRecvThread(*m_udpSocket);
}

void CIOCPServerApp::OnCleanup()
{
	if (m_udpSocket)	{ delete m_udpSocket; m_udpSocket = nullptr; }
	if (m_udpThread)	{ WaitForSingleObject(m_udpThread->GetHandle(), true); delete m_udpThread; m_udpThread = nullptr; }
	if (m_iocp)			{ delete m_iocp; m_iocp = nullptr; }
	if (m_listener)		{ delete m_listener; m_listener = nullptr; }
	if (m_channel)		{ delete m_channel; m_channel = nullptr; }
	if (m_inServerUserManager) { m_inServerUserManager->ReleaseAll(); delete m_inServerUserManager; m_inServerUserManager = nullptr; }
}

void CIOCPServerApp::ResFailEnteredServer(CUser* _user)
{
	CMemoryStream memoryStream(10);
	memoryStream.Write(static_cast<u_short>(g_headerSize + sizeof(byte)));
	memoryStream.Write(static_cast<u_short>(eTcpPacketType::S2C_EnterServerResult));
	memoryStream.Write(static_cast<byte>(false));

	_user->SendPacket(memoryStream.GetBuff());

	_user->Disconnect();
	delete _user;
}

void CIOCPServerApp::ResSuccessEnteredServer(CUser* _user)
{
	CMemoryStream memoryStream(10);
	memoryStream.Write(static_cast<u_short>(g_headerSize + sizeof(byte)));
	memoryStream.Write(static_cast<u_short>(eTcpPacketType::S2C_EnterServerResult));
	memoryStream.Write(static_cast<byte>(true));

	_user->SendPacket(memoryStream.GetBuff());

	if (!_user->RegisterIOCP(m_iocp)) 
	{
		_user->Disconnect();
		delete _user;
	}
}