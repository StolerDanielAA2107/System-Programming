#pragma once 
#include "pch.h"

enum MessageTypes
{
	MT_CONNECT,
	MT_DISCONNECT,
	MT_GETDATA,
	MT_DATA,
	MT_CLIENTS_LIST
};


struct MessageHeader
{
	int to; //������������� ����������
	int from; // ������������� �����������
	int messageType; // ��� ���������
	int size; // ������ ���������
};

class Message
{
public:
	MessageHeader header = { 0 };
	string data;
	static int clientID;

	Message() {}

	Message(int type, int to = 0, const string& data = "", int from = clientID)
	{
		this->data = data;
		header = { to, from, type, int(this->data.length()) };
	}

	void send(CSocket& s)
	{
		s.Send(&header, sizeof(MessageHeader));		// �������� ��������� ���������
		if (header.size)    // ��������, ���� �� ������ ��� �������� (������ ������ ������ ����)
		{
			s.Send(data.c_str(), (int)header.size);        // �������� ������ ���������

		}
	}

	int receive(CSocket& s)
	{
		// ������� �������� ��������� ��������� � �������� ��� �������
		if (s.Receive(&header, sizeof(MessageHeader)) && header.size)
		{
			// ���� ��������� ������� ������� � ������ ��������� ���������
			vector <char> v(header.size);// �������� ������� ��� �������� ������ ���������
			s.Receive(&v[0], (int)header.size); // ��������� ������ ���������
			data = string(&v[0], header.size); // �������������� ���������� ������ � ������
		}
		return header.messageType; // ����������� ���� ���������
	}
};