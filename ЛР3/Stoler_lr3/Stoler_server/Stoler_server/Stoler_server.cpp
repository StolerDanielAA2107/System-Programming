#include "pch.h"
#include "framework.h"
#include "Stoler_server.h"
#include <afxsock.h>
#include <vector>
#include <string>
#include <thread>
#include "Session.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;


std::vector<Session*> sessions;

struct header {
    int id;
    int commandId;
    int size;
};


void thread2(Session* ses) {
    std::cout << "Session " << ses->id << std::endl;
    while (true) {
        Message m;
        if (ses->getMessages(m)) {
            if (m.mHeader.mType == MTYPE_CLOSE) {
                EnterCriticalSection(&ses->section);
                std::cout << "Session " << ses->id << " closed" << std::endl;
                LeaveCriticalSection(&ses->section);
                auto it = std::find_if(sessions.begin(), sessions.end(), [ses](Session* s) {
                    return ses->id == s->id;
                });
                sessions.erase(it);
                delete ses;
                break;
            }
            else if (m.mHeader.mType == MTYPE_INFO) {
                std::string filename = std::to_string(ses->id) + ".txt";
                HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
                const char* buffer = m.data.c_str();

                WriteFile(hFile, buffer, strlen(buffer), NULL, NULL);

                CloseHandle(hFile);

                std::cout << "Session " << ses->id << " data " << m.data << std::endl;
            }
        }

    }
}

void threadSock(SOCKET sock) {
    CSocket c;
    c.Attach(sock);
    while (true) {
        int nLength;
        header h;
        c.Receive(&h, sizeof(header));
        switch (h.commandId)
        {
            case 1:
            {
                int k = h.id;
                for (int i = 0; i < k; i++) {
                    Session* ses = new Session(sessions.size() + 1);
                    sessions.push_back(ses);
                    std::thread t(thread2, ses);
                    t.detach();
                }
                break;
            };
            case 2: 
            {
                int id = h.id;
                if (id == -1) {
                    for (auto s : sessions) {
                        Message message(MTYPE_CLOSE);
                        s->sendMessage(message);
                    }
                    break;
                }
                auto it = std::find_if(sessions.begin(), sessions.end(), [id](Session* s) {
                    return s->id == id;
                });
                Message message(MTYPE_CLOSE);
                Session* lastSess = *it;
                lastSess->sendMessage(message);
                break;
            };
            case 3:
            {
                
                std::vector <char> v(h.size);
                c.Receive(&v[0], h.size);
                std::string m(&v[0], h.size);
                int id = h.id;
                if (id == -1) {
                    for (auto s : sessions) {
                        Message message(MTYPE_INFO, m);
                        s->sendMessage(message);
                    }
                    break;
                }
                auto it = std::find_if(sessions.begin(), sessions.end(), [id](Session* s) {
                    return s->id == id;
                });
                Session* lastSess = *it;
                Message message(MTYPE_INFO, m);
                lastSess->sendMessage(message);
                break;
            };
            case 4: 
            {
                std::string response;
                for (auto s : sessions) {
                    response = response + std::to_string(s->id) + "|";
                }
                int s = response.size();
                c.Send(&s, sizeof(s));
                c.Send(response.c_str(), s);
                break;
            };
        }
    }
    c.Close();
}

int main()
{
    int nRetCode = 0;
    setlocale(LC_ALL, "Russian");
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
            AfxSocketInit();
            CSocket server;
            server.Create(1111);
            while (true) {
                std::cout << "Server started" << std::endl;
                if (!server.Listen()) // режим прослушивания входящих сообщений
                    break;
                CSocket c;
                if (server.Accept(c)) {
                    std::thread t(threadSock, c.Detach());
                    t.detach();
                }
                
            }
        }
    }
    else
    {
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
