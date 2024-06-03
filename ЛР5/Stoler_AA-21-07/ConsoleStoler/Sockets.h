#pragma once
#include "framework.h"
#include "Message.h"
#include <vector>

#include <afxsock.h>



inline Message receiveMessage(CSocket& s)
{
	Message message;
	MessageHeader mh;

	s.Receive(&mh, sizeof(MessageHeader));
	message.header = mh;

	if (mh.size == 0)
		return message;

	std::vector<char> v(mh.size);
	s.Receive(&v[0], mh.size);

	message.data = std::string(&v[0], mh.size);

	return message;
}

inline void sendMessage(CSocket& s, const Message& mess)
{
	s.Send(&mess.header, sizeof(MessageHeader));
	if (mess.data.size() == 0)
		return;
	s.Send(mess.data.c_str(), mess.data.size());
}


