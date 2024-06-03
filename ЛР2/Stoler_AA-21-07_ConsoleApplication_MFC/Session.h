#pragma once
#include "Message.h"


class Session
{
private:
	std::queue<Message> messages;//очередь FIFO из сообщений


	CRITICAL_SECTION cs;
	/*CCriticalSection - это класс, предоставляемый Microsoft Foundation Class (MFC) Library,
	который используется для синхронизации доступа к общим ресурсам в многопоточной среде.
	Он предотвращает одновременный доступ к общему ресурсу, что может привести к состоянию гонки.*/


public:
	int sessionID;

	Session(int sessionID)
		: sessionID{ sessionID }
	{


		InitializeCriticalSection(&cs);// инициализирую критическую секцию


	}

	~Session()
	{


		DeleteCriticalSection(&cs);// удаляю критическую секцию


	}

	bool getMessage(Message& m);
	void addMessage(Message& m);
	void addMessage(MessageTypes messageType, const string& data = "");
};
