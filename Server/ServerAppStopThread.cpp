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
            key = _getch();  // 입력된 키를 가져옴

            if (key == VK_ESCAPE)
            {
                m_listener.Close();
                break;
            }
        }
    }
}