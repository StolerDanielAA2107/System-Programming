#pragma once
#include"../Stoler_AA-21-07_Library_MFC/Message.h"



class Session
{
public:
	int sessionID;
	string name;											// FIFO гарантирует, что сообщения будут обработаны
	queue<Message> messages;//очередь FIFO из сообщений		// в том порядке, в котором они оступили. 
	CRITICAL_SECTION cs;									// Это особенно важно для сетевых приложений,
															// где порядок сообщений может влиять на логику 
	Session(int _sessionID, string _name)					// приложения и консистентность данных.
		: sessionID{ _sessionID }, name{ _name }
	{
		InitializeCriticalSection(&cs);
	}

	~Session()
	{
		DeleteCriticalSection(&cs);
	}

	void getMessage(CSocket& s); // Получение сообщения из сокета
	void addMessage(Message& m); // Метод для добавления сообщения в очередь
};
