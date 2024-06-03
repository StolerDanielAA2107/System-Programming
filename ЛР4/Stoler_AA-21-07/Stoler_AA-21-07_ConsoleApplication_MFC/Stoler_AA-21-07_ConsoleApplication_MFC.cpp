#include "pch.h"
#include "framework.h"

#include "Stoler_AA-21-07_ConsoleApplication_MFC.h"
#include "Session.h"
#include "Writing.h"
#include <map>
#include <afxsock.h>
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Единственный объект приложения
CWinApp theApp;

int Message::clientID = 0;
int maxID = 0;
map<int, shared_ptr<Session>> sessions;
static int threadCounter = 0;

// Функция для отправки списка клиентов всем подключенным клиентам
void sendClientListToAll()
{
	string ids = "";
	// Формирование строки с идентификаторами всех сессий
	for_each(sessions.begin(), sessions.end(), [&ids](pair<const int, std::shared_ptr<Session>> session)
		{
			ids += std::to_string(session.second->sessionID) + "|";
		});
	// Создание сообщения с типом MT_CLIENTS_LIST и отправка его всем сессиям
	Message m(MT_CLIENTS_LIST, -1, ids);
	for (auto& [id, session] : sessions)
	{
		session->addMessage(m);
	}
}

// Функция обработки соединения в отдельном потоке
void threadSock(SOCKET hSock)
{
	CSocket socket;
	socket.Attach(hSock);

	Message m;
	int code = m.receive(socket);

	switch (code)
	{
	case MT_CONNECT:
	{
		// Создание новой сессии и добавление её в список сессий
		auto session = make_shared<Session>(++maxID, m.data);
		sessions[session->sessionID] = session;

		// Отправка клиенту его нового идентификатора сессии
		m.header.to = session->sessionID;
		m.send(socket);

		// Логирование и отправка обновленного списка клиентов всем
		SafeWrite(("Тип запроса: MT_CONNECT, ID:" + to_string(session->sessionID)));
		sendClientListToAll();

		break;
	}
	case MT_DISCONNECT:
	{
		// Логирование и удаление сессии из списка сессий
		SafeWrite(("Тип запроса: MT_DISCONNECT, ID:" + to_string(m.header.from)));
		sessions.erase(m.header.from);

		// Отправка обновленного списка клиентов всем
		sendClientListToAll();
		break;
	}
	case MT_GETDATA:
	{
		SafeWrite(("Тип запроса: MT_GETDATA, для: ") + to_string(m.header.from));
		auto iSession = sessions.find(m.header.from);
		if (iSession != sessions.end())
		{
			iSession->second->getMessage(socket);
		}
		break;
	}
	case MT_DATA:
	{
		// Обработка сообщения с данными
		auto iSessionFrom = sessions.find(m.header.from);
		if (iSessionFrom != sessions.end())
		{
			auto iSessionTo = sessions.find(m.header.to);
			if (iSessionTo != sessions.end())
			{
				// Логирование и добавление сообщения в очередь получателя
				SafeWrite(("Тип запроса: MT_DATA, для: " + to_string(m.header.to) + ", от: " + to_string(m.header.from)));
				iSessionTo->second->addMessage(m);
			}
			else if (m.header.to == -1)
			{
				// Логирование и рассылка сообщения всем, кроме отправителя
				SafeWrite(("Тип запроса: MT_DATA, для всех, от: " + to_string(m.header.from)));
				for (auto& [id, session] : sessions)
				{
					if (id != m.header.from)
						session->addMessage(m);
				}
			}
		}
		break;
	}
	default:
	{
		return;
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
			setlocale(LC_ALL, "Russian");
			AfxSocketInit();//инициируем библиотеку сокетов ВинСок

			CSocket server;//создаём объект, который будет использоваться для управления серверным сокетом
			server.Create(12345);//создаём серверный сокет, привязанный к порту 12345
			cout << "Сервер запущен." << endl;

			while (true) 
			{
				if (!server.Listen())//проверяем возможность порта принимать клиентов
					break;
				CSocket c;
				// Ожидание подключения клиента
				if (server.Accept(c)) 
				{
					// Создание нового потока для обработки клиента
					thread t(threadSock, c.Detach());//создаём новый поток, обрабатывающий подключение и выполняющий threadSock
					//при этом мы отключаем поток c от новосозданного
					t.detach();//а теперь отключаем новосозданный поток. Это значит мы не будем ждать окончания выполнения потока
					// t и программа продолжит выполнение 
				}
			}
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
