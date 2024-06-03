#pragma once
#include "Message.h"


class Session
{
private:
	std::queue<Message> messages;//������� FIFO �� ���������


	CRITICAL_SECTION cs;
	/*CCriticalSection - ��� �����, ��������������� Microsoft Foundation Class (MFC) Library,
	������� ������������ ��� ������������� ������� � ����� �������� � ������������� �����.
	�� ������������� ������������� ������ � ������ �������, ��� ����� �������� � ��������� �����.*/


public:
	int sessionID;

	Session(int sessionID)
		: sessionID{ sessionID }
	{


		InitializeCriticalSection(&cs);// ������������� ����������� ������


	}

	~Session()
	{


		DeleteCriticalSection(&cs);// ������ ����������� ������


	}

	bool getMessage(Message& m);
	void addMessage(Message& m);
	void addMessage(MessageTypes messageType, const string& data = "");
};
