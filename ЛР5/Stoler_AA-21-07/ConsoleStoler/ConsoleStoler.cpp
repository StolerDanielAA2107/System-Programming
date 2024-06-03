#include "pch.h"
#include "framework.h"
#include "ConsoleStoler.h"
#include "Session.h"
#include "Sockets.h"
#include "SafeWriting.h"
#include <map>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int maxID = MR_USER;
std::map<int, shared_ptr<Session>> sessions;
static int threadCounter = 0;

void sendClientListToAll()
{
	std::string ids = "";
	std::for_each(sessions.begin(), sessions.end(), [&ids](std::pair<const int, std::shared_ptr<Session>> session)
		{
			ids += std::to_string(session.second->id) + ",";
		});
	Message m(MR_ALL, MR_BROKER, MT_CLIENTS_LIST, ids);
	for (auto& [id, session] : sessions)
	{
		session->add(m);
	}
}

void ProcessClient(SOCKET hSock)
{
	CSocket socket;
	socket.Attach(hSock);
	Message m;
	int code = m.receive(socket);
	cout << m.header.to << ": " << m.header.from << ": " << m.header.messageType << ": " << code << endl;

	switch (code)
	{
	case MT_INIT:
	{
		auto session = make_shared<Session>(++maxID, m.data);
		sessions[session->id] = session;
		Message::send(socket, session->id, MR_BROKER, MT_INIT);
		sendClientListToAll();
		
		break;
	}
	case MT_EXIT:
	{
		sessions.erase(m.header.from);
		Message::send(socket, m.header.from, MR_BROKER, MT_CONFIRM);
		sendClientListToAll();
		break;
	}
	case MT_GETDATA:
	{
		auto iSession = sessions.find(m.header.from);
		if (iSession != sessions.end())
		{
			iSession->second->send(socket);
		}
		break;
	}
	default:
	{
		auto iSessionFrom = sessions.find(m.header.from);
		if (iSessionFrom != sessions.end())
		{
			auto iSessionTo = sessions.find(m.header.to);
			if (iSessionTo != sessions.end())
			{
				iSessionTo->second->add(m);
			}
			else if (m.header.to == MR_ALL)
			{
				for (auto& [id, session] : sessions)
				{
					if (id != m.header.from)
						session->add(m);
				}
			}			
		}
		break;
	}
	}
	
}

void LaunchClient()
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	::CreateProcess(NULL, (LPSTR)"systemProgLab1.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	::CloseHandle(pi.hThread);
	::CloseHandle(pi.hProcess);
}

void Server()
{
	AfxSocketInit();

	CSocket Server;
	Server.Create(22002);

	while (true)
	{
		if (!Server.Listen())
			break;
		CSocket s;
		Server.Accept(s);
		SafeWrite("Request Accepted ");
		thread t(ProcessClient, s.Detach());
		t.detach();
	}
}


int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {

        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
			Server();
        }
    }
    else
    {
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
