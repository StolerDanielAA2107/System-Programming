// Stoler_AA-21-07_ConsoleApplication_MFC.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include "framework.h"

#include "Stoler_AA-21-07_ConsoleApplication_MFC.h"
#include "Session.h"
#include "Writing.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Единственный объект приложения
CWinApp theApp;

string message = "";
int targetThread;

UINT workFunc(LPVOID  _param)
{
	auto session = static_cast<Session*>(_param);// создаём сессию
	SafeWrite("Поток", session->sessionID + 1, "создан"); //выводим сообщение о создании 

	while (true)
	{
		Message m;
		if (session->getMessage(m))
		{
			switch (m.header.messageType)
			{
			case MT_CLOSE:
			{
				closeClose(session->sessionID);//удаляем дескриптор потока
				popBackCloseEvent();//чистим очередь потоков
				SafeWrite("Поток", session->sessionID + 1, "закрылся");
				delete session;
				return 0;
			}
			case MT_DATA:
			{
				SafeWrite("Поток:", session->sessionID, "вывел сообщение:", m.data);
				SafeFile(to_string(session->sessionID + 1) + ".txt", message);
				Sleep(100 * session->sessionID);
				break;
			}
			}
		}
	}
	return 0;
}

int start()
{
	vector<Session*> sessions; //вектор сессий
	SafeWrite("Главный поток создан.");
	//Сразу устанавлимаем событие confirmEvent в сигнальное состояние/чтобы оповестить c# о запуске приложения
	setEventConfirm();
	
	while (true)
	{
		int evNum = getCurrentEvent();//ожидаем одного из 4х событий
		switch (evNum)
		{
		case 0://добавление
		{
			sessions.push_back(new Session(getCloseEventSize())); //создаём и добавляем новую сессию
			addSessionEvent();
			addCloseEvent();//Создаём новый поток
			addMessageEvent();//Создаём новый поток сообщений

			AfxBeginThread(workFunc, (LPVOID)(sessions.back()));//запускаем поток
			break;
		}
		case 1://удаление
		{
			if (closeEventIsEmpty()) //если есть элементы удалим один поток
			{
				sessions.back()->addMessage(MT_CLOSE);//выводим сообщение о закрытии
				sessions.pop_back();//удаляем последнию сессию
				break;
			}
		}
		case 2://выход
		{
			for (int i = sessions.size() - 1; i >= 0; --i)
			{
				sessions[i]->addMessage(MT_CLOSE);
			}
			sessions.clear();
			deleteSessionEvents();
			deleteCloseEvents();
			deleteMessageEvents();
			setEventConfirm();
			deleteEvents();

			return 0;
		}
		case 3:
		{
			int targetThread;
			char* tmp = getMessage(targetThread); //получаем текст сообщения и номер потока
			message = string(tmp);//загоняем текст в глобальную переменную
			delete[] tmp;

			if (targetThread != -1)//в зависимости от номера или выведится одно сообщение и отрисуется один файл, или одно, но вывод будет во все файлы
			{
				sessions[targetThread]->addMessage(MT_DATA, message);
			}
			else
			{
				for (int i = sessions.size() - 1; i >= 0; --i)
				{
					sessions[i]->addMessage(MT_DATA, message);
				}
			}
			break;
		}
		default:
			std::cout << "Критическая ошибка!" << std::endl;
			return 0;
		}
		setEventConfirm();//оповещаем c# о создании объекта
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
			start();
            // TODO: вставьте сюда код для приложения.
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
