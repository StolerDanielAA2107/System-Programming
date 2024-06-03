#include "pch.h"
#include "Session.h"

bool Session::getMessage(Message& m)
{
	bool res = false;
	waitSession(this->sessionID);


	EnterCriticalSection(&cs);

	if (!messages.empty())
	{
		res = true;
		m = messages.front();
		messages.pop();
	}
	if (messages.empty())
	{
		resetSession(this->sessionID);
	}

	LeaveCriticalSection(&cs);


	return res;
}


void Session::addMessage(Message& m)//добавляем сообщения в очередь 
{

	EnterCriticalSection(&cs);//входим в критическую секцию
	messages.push(m);//добавляем в очередь
	setSession(this->sessionID);
	LeaveCriticalSection(&cs);//выходим из критической секции

}

void Session::addMessage(MessageTypes messageType, const string& data)
{
	Message m(messageType, data);
	addMessage(m);
}
