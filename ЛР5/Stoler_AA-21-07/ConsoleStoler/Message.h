#pragma once


enum MessageTypes
{
	MT_INIT,
	MT_EXIT,
	MT_GETDATA,
	MT_DATA,
	MT_NODATA,
	MT_CONFIRM,
	MT_CLIENTS_LIST
};


enum MessageRecipients
{
	MR_BROKER = 10,
	MR_ALL = 50,
	MR_USER = 100
};


struct MessageHeader
{
	int to;
	int from;
	int messageType;
	int size;
};


class Message
{
public:
	MessageHeader header = { 0 };
	std::string data;
	static int clientID;

	Message() {}

	Message(int to, int from, int type = MT_DATA, const std::string& data = "")
	{
		this->data = data;
		header = { to, from, type, int(data.length()) };
	}

	void send(CSocket& s)
	{
		s.Send(&header, sizeof(MessageHeader));
		if (header.size)
		{
			s.Send(data.c_str(), (int)header.size);
		}
	}

	int receive(CSocket& s)
	{
		if (!s.Receive(&header, sizeof(MessageHeader)))
		{
			return MT_NODATA;
		}
		if (header.size)
		{
			std::vector <char> v(header.size);
			s.Receive(&v[0], (int)header.size);
			data = std::string(&v[0], header.size);
		}
		return header.messageType;
	}

	static void send(CSocket& s, int to, int from, int type = MT_DATA, const std::string& data = "");
	static Message send(int to, int type = MT_DATA, const std::string& data = "");

};
