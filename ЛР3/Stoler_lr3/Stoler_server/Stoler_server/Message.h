#pragma once
#include <string>
enum MessageType{
	MTYPE_CLOSE,
	MTYPE_INFO
};


struct MessageHeader {
	size_t mSize;
	MessageType mType;
};


class Message
{
public:
	MessageHeader mHeader;
	std::string data;
	Message(MessageType _mType, const std::string &mData = "") {
		mHeader = { sizeof(mData) * mData.size(), _mType };
		data = mData;
	}

	Message() = default;
	
	~Message() {

	}

};

