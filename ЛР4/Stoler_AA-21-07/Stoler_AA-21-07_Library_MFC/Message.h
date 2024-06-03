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
	int to; //идентификатор получателя
	int from; // идентификатор отправителя
	int messageType; // тип сообщения
	int size; // размер сообщения
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
		s.Send(&header, sizeof(MessageHeader));		// Отправка заголовка сообщения
		if (header.size)    // Проверка, есть ли данные для отправки (размер данных больше нуля)
		{
			s.Send(data.c_str(), (int)header.size);        // Отправка данных сообщения

		}
	}

	int receive(CSocket& s)
	{
		// Попытка получить заголовок сообщения и проверка его размера
		if (s.Receive(&header, sizeof(MessageHeader)) && header.size)
		{
			// Если заголовок успешно получен и размер сообщения ненулевой
			vector <char> v(header.size);// Создание вектора для хранения данных сообщения
			s.Receive(&v[0], (int)header.size); // Получение данных сообщения
			data = string(&v[0], header.size); // Преобразование полученных данных в строку
		}
		return header.messageType; // Возвращение типа сообщения
	}
};