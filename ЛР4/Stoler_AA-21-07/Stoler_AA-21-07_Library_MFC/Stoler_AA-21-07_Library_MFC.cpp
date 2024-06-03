#include "pch.h"
#include "framework.h"
#include "Stoler_AA-21-07_Library_MFC.h"
#include "Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <afxsock.h>
//
//TODO: если эта библиотека DLL динамически связана с библиотеками DLL MFC,
//		все функции, экспортированные из данной DLL-библиотеки, которые выполняют вызовы к
//		MFC, должны содержать макрос AFX_MANAGE_STATE в
//		самое начало функции.
//
//		Например:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// тело нормальной функции
//		}
//
//		Важно, чтобы данный макрос был представлен в каждой
//		функции до вызова MFC.  Это означает, что
//		должен стоять в качестве первого оператора в
//		функции и предшествовать даже любым объявлениям переменных объекта,
//		поскольку их конструкторы могут выполнять вызовы к MFC
//		DLL.
//
//		В Технических указаниях MFC 33 и 58 содержатся более
//		подробные сведения.
//


CSocket s;

BOOL CStolerAA2107LibraryMFCApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}


int Message::clientID = 0;

void SocConnect(CSocket& Socket)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	AfxSocketInit(); // Инициализация сокетов MFC
	Socket.Create(); // Создание сокета
	Socket.Connect("127.0.0.1", 12345); // Подключение к серверу по IP-адресу и порту
}

extern "C"
{
	__declspec(dllexport) int Connect(int port = 12345)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());


		CSocket Socket;
		SocConnect(Socket);

		// Создание и отправка сообщения о подключении
		Message m(MT_CONNECT);
		m.send(Socket);

		// Получение ответа от сервера
		m.receive(Socket);

		// Установка clientID из ответа сервера
		Message::clientID = m.header.to;

		return Message::clientID;
	}


	__declspec(dllexport) void Disconnect()
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		CSocket Socket;
		SocConnect(Socket);

		Message m(MT_DISCONNECT);
		m.send(Socket);
	}


	__declspec(dllexport) void sendMessageTo(int addr, const char* str)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		CSocket Socket;
		SocConnect(Socket);


		Message m(MT_DATA, addr, str);
		m.send(Socket);
		m.receive(Socket);// Получение подтверждения или ответа от сервера
	}

	// Обработка сообщений
	__declspec(dllexport) int ProcessMessages(int maxBufferLength, int& mes_code, char* buffer)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		CSocket Socket;
		SocConnect(Socket);

		// Создание и отправка запроса на получение данных
		Message m(MT_GETDATA);
		m.send(Socket);
		m.receive(Socket);

		switch (m.header.messageType)
		{
		case MT_DATA:
		{
			mes_code = MT_DATA;
			if (m.header.size > 0)
			{
				// Копирование данных сообщения в буфер
				memcpy_s(buffer, m.header.size, m.data.c_str(), m.header.size);
				return m.header.size; // Возвращение размера данных
			}
			break;
		}
		case MT_CLIENTS_LIST:
		{
			if (m.header.size > 1)
			{
				mes_code = MT_CLIENTS_LIST;
				memcpy_s(buffer, maxBufferLength, m.data.c_str(), m.data.size() - 1);
				return m.data.size() - 1; // Возвращение размера списка клиентов
			}
			break;
		}
		}
		return 0;
	}
}