#include "pch.h"
#include "Session.h"

void Session::getMessage(CSocket& s)
{

	EnterCriticalSection(&cs);
	if (!messages.empty())
	{
		messages.front().send(s);
		messages.pop();
	}
	LeaveCriticalSection(&cs);
}


void Session::addMessage(Message& m)//добавляем сообщения в очередь 
{
	EnterCriticalSection(&cs);//входим в критическую секцию
	messages.push(m);//добавляем в очередь
	LeaveCriticalSection(&cs);//выходим из критической секции
}