#include "ServerAppStopThread.h"
#include <conio.h>
CServerAppStopThread::CServerAppStopThread(CTCPListener& _listener)
	:m_listener(_listener)
{

}

CServerAppStopThread::~CServerAppStopThread()
{

}

void CServerAppStopThread::Run()
{
    while (true) 
    {
        char key = NULL;
        if (_kbhit()) 
        {  
            key = _getch();  // �Էµ� Ű�� ������

            if (key == VK_ESCAPE)
            {
                m_listener.Close();
                break;
            }
        }
    }
}