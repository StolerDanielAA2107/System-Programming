#pragma once
#include"../Stoler_AA-21-07_Library_MFC/Message.h"



class Session
{
public:
	int sessionID;
	string name;											// FIFO �����������, ��� ��������� ����� ����������
	queue<Message> messages;//������� FIFO �� ���������		// � ��� �������, � ������� ��� ��������. 
	CRITICAL_SECTION cs;									// ��� �������� ����� ��� ������� ����������,
															// ��� ������� ��������� ����� ������ �� ������ 
	Session(int _sessionID, string _name)					// ���������� � ��������������� ������.
		: sessionID{ _sessionID }, name{ _name }
	{
		InitializeCriticalSection(&cs);
	}

	~Session()
	{
		DeleteCriticalSection(&cs);
	}

	void getMessage(CSocket& s); // ��������� ��������� �� ������
	void addMessage(Message& m); // ����� ��� ���������� ��������� � �������
};
