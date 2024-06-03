// LR1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include "framework.h"
#include "LR1.h"
#include <queue>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Единственный объект приложения

CWinApp theApp;

using namespace std;

enum MessageTypes
{
	MT_CLOSE,
	MT_DATA,
};

struct MessageHeader
{
	int messageType;
	int size;
};

struct Message
{
	MessageHeader header = { 0 };
	string data;
	Message() = default;
	Message(MessageTypes messageType, const string& data = "")
		:data(data)
	{
		header = { messageType,  int(data.length()) };
	}
};

class Session
{
	queue<Message> messages;
	CRITICAL_SECTION cs;
	HANDLE hEvent;
public:
	int sessionID;

	Session(int sessionID)
		:sessionID(sessionID)
	{
		InitializeCriticalSection(&cs);
		hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	~Session()
	{
		DeleteCriticalSection(&cs);
		CloseHandle(hEvent);
	}

	void addMessage(Message& m)
	{
		EnterCriticalSection(&cs);
		messages.push(m);
		SetEvent(hEvent);
		LeaveCriticalSection(&cs);
	}

	bool getMessage(Message& m)
	{
		bool res = false;
		WaitForSingleObject(hEvent, INFINITE);
		EnterCriticalSection(&cs);
		if (!messages.empty())
		{
			res = true;
			m = messages.front();
			messages.pop();
		}
		if (messages.empty())
		{
			ResetEvent(hEvent);
		}
		LeaveCriticalSection(&cs);
		return res;
	}

	void addMessage(MessageTypes messageType, const string& data = "")
	{
		Message m(messageType, data);
		addMessage(m);
	}
};

DWORD WINAPI MyThread(LPVOID lpParameter)
{
	auto session = static_cast<Session*>(lpParameter);
	int ID = session->sessionID;
	cout << "Поток " << ID << " создан" << endl;
	while (true)
	{
		Message m;
		if (session->getMessage(m))
		{
			switch (m.header.messageType)
			{
			case MT_CLOSE:
			{
				cout << "Поток " << ID << " завершен" << endl;
				delete session;
				return 0;
			}
			}
		}
	}
	return 0;
}

void start()
{
	cout << "work" << endl;
	vector<Session*> sessions;
	vector<HANDLE> threads;

	HANDLE event_confirm = CreateEvent(NULL, FALSE, FALSE, "event_confirm");

	HANDLE Events[3];
	Events[0] = CreateEvent(NULL, FALSE, FALSE, "event_start");
	Events[1] = CreateEvent(NULL, FALSE, FALSE, "event_stop");
	Events[2] = CreateEvent(NULL, FALSE, FALSE, "event_close");

	bool exit = true;

	while (exit)
	{
		int evnum = WaitForMultipleObjects(3, Events, FALSE, INFINITE) - WAIT_OBJECT_0;
		switch(evnum)
		{
			case 0: //start
			{
				sessions.push_back(new Session(sessions.size() + 1));
				SetEvent(event_confirm);
				break;
			}
			case 1: //close
			{
				if (!threads.empty() && !sessions.empty())
				{
					sessions.back()->addMessage(MT_CLOSE);
					sessions.pop_back();
					SetEvent(event_confirm);
					break;
				}
			}
			case 2: //exit
			{
				threads.clear();
				sessions.clear();
				exit = false;
				break;
			}
		}
	}
}

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // инициализировать MFC, а также печать и сообщения об ошибках про сбое
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: вставьте сюда код для приложения.
            wprintf(L"Критическая ошибка: сбой при инициализации MFC\n");
            nRetCode = 1;
        }
        else
        {
			start();
		}
    }
    else
    {
        // TODO: измените код ошибки в соответствии с потребностями
        wprintf(L"Критическая ошибка: сбой GetModuleHandle\n");
        nRetCode = 1;
    }

    return nRetCode;
}
