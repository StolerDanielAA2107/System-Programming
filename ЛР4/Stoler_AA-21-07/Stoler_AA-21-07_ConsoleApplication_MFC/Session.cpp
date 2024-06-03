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


void Session::addMessage(Message& m)//��������� ��������� � ������� 
{
	EnterCriticalSection(&cs);//������ � ����������� ������
	messages.push(m);//��������� � �������
	LeaveCriticalSection(&cs);//������� �� ����������� ������
}