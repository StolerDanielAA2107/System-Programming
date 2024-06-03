#pragma once
#define _AFXDLL

#include <afxmt.h>

#include <iostream>

inline void DoWrite()
{
	std::cout << std::endl;
}

template <class T, typename... Args> inline void DoWrite(T& value, Args... args)
{
	std::cout << value << " ";
	DoWrite(args...);
}

static CCriticalSection cs;
template <typename... Args> inline void SafeWrite(Args... args)
{
	cs.Lock();
	DoWrite(args...);
	cs.Unlock();
}

