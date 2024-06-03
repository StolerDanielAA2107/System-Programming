#pragma once
#include "pch.h"

//Функция для выхода из бесконечного цикла DoWrite, вызванного передачей множественных аргументов
inline void DoWrite()
{
	cout << endl;
}
//Выводим первый из передаваемых аргументов аргумент, а затем передаём сюда же оставшиеся аргументы, до тех пор, пока они не кончатся
template <class T, typename... Args> inline void DoWrite(T& value, Args... args)
{
	cout << value << " ";
	DoWrite(args...);
}

//static CCriticalSection cs;
template <typename... Args> inline void SafeWrite(Args... args)
{
	waitMutex();
	//cs.Lock();
	//блокируем критическую секцию, для того чтобы потоки не могли мешать действующему
	// потоку, пока тот выполняет код до строчки cs.Unlock();
	/*
	for (int i = 0; i < 1000; ++i)
		std::cout << i<<" ";
	std::cout << std::endl;
	*/
	DoWrite(args...);
	releaseMutex();
	//cs.Unlock();
}

void SafeFile(string fName, string& message);

/*
void SafeFile(string fName, string& message)
{
	//waitMutex();
	cs.Lock();
	ofstream fout(fName);
	fout << message;
	fout.close();
	//releaseMutex();
	cs.Unlock();
}
*/
