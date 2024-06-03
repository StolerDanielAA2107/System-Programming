#pragma once
#include "pch.h"

//������� ��� ������ �� ������������ ����� DoWrite, ���������� ��������� ������������� ����������
inline void DoWrite()
{
	cout << endl;
}
//������� ������ �� ������������ ���������� ��������, � ����� ������� ���� �� ���������� ���������, �� ��� ���, ���� ��� �� ��������
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
	//��������� ����������� ������, ��� ���� ����� ������ �� ����� ������ ������������
	// ������, ���� ��� ��������� ��� �� ������� cs.Unlock();
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
