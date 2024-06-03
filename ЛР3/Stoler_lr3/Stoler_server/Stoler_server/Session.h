#pragma once
#include <queue>
#include <Windows.h>
#include "Message.h"

class Session {
private:
	std::queue<Message> messages = {};

public:
	CRITICAL_SECTION section;
	int id;
	HANDLE hEvent;

	Session(int _id) : id(_id) {
		hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		InitializeCriticalSection(&section);
	}

	bool getMessages(Message& m) {
		bool result = false;
		WaitForSingleObject(hEvent, 0);
		if (messages.size() != 0) {
			m = messages.front();
			messages.pop();
			result = true;
		}
		if (messages.size() == 0) {
			ResetEvent(hEvent);
		}
		return result;
	}

	void sendMessage(Message &m) {
		messages.push(m); 
		SetEvent(hEvent);
	}

	~Session() {
		DeleteCriticalSection(&section);
		CloseHandle(hEvent);
	}
};
