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


void Session::addMessage(Message& m)//��������� ��������� � ������� 
{

	EnterCriticalSection(&cs);//������ � ����������� ������
	messages.push(m);//��������� � �������
	setSession(this->sessionID);
	LeaveCriticalSection(&cs);//������� �� ����������� ������

}

void Session::addMessage(MessageTypes messageType, const string& data)
{
	Message m(messageType, data);
	addMessage(m);
}
