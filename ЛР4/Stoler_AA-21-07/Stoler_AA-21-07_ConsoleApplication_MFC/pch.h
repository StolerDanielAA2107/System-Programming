// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.

#ifndef PCH_H
#define PCH_H

// Добавьте сюда заголовочные файлы для предварительной компиляции
#include "framework.h"
#include <Windows.h>
#include <map>
#include <queue>
#include <iostream>
#include <fstream>

#include <afxmt.h>
#include <afxsock.h>

#include <string>

#include <sstream>
#include <queue>
#include <vector>
#include <thread>
using namespace std;

extern "C" {
	__declspec(dllimport) void __stdcall deleteEvents();
	__declspec(dllimport) int __stdcall getCurrentEvent();
	__declspec(dllimport) void __stdcall setEventConfirm();
	__declspec(dllimport) void __stdcall releaseMutex();
	__declspec(dllimport) void __stdcall waitMutex();
	__declspec(dllimport) void __stdcall addCloseEvent();
	__declspec(dllimport) void __stdcall addMessageEvent();
	__declspec(dllimport) boolean __stdcall closeEventIsEmpty();
	__declspec(dllimport) void __stdcall deleteCloseEvents();
	__declspec(dllimport) int __stdcall getCloseEventSize();
	__declspec(dllimport) char* __stdcall getMessage(int& threadNumber);
	__declspec(dllimport) void __stdcall deleteMessageEvents();
	__declspec(dllimport) void __stdcall popBackCloseEvent();
	__declspec(dllimport) void __stdcall setSession(int threadNumber);
	__declspec(dllimport) void __stdcall resetSession(int threadNumber);
	__declspec(dllimport) void __stdcall waitSession(int threadNumber);
	__declspec(dllimport) void __stdcall closeClose(int threadNumber);
	__declspec(dllimport) void __stdcall deleteSessionEvents(); 
	__declspec(dllimport) void __stdcall addSessionEvent();
}

#endif //PCH_H
