// Stoler_AA-21-07_Library_MFC.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "pch.h"
#include "framework.h"
#include "Stoler_AA-21-07_Library_MFC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//
//TODO: если эта библиотека DLL динамически связана с библиотеками DLL MFC,
//		все функции, экспортированные из данной DLL-библиотеки, которые выполняют вызовы к
//		MFC, должны содержать макрос AFX_MANAGE_STATE в
//		самое начало функции.
//
//		Например:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// тело нормальной функции
//		}
//
//		Важно, чтобы данный макрос был представлен в каждой
//		функции до вызова MFC.  Это означает, что
//		должен стоять в качестве первого оператора в
//		функции и предшествовать даже любым объявлениям переменных объекта,
//		поскольку их конструкторы могут выполнять вызовы к MFC
//		DLL.
//
//		В Технических указаниях MFC 33 и 58 содержатся более
//		подробные сведения.
//


struct Description
{
	int size;
	int threadNumber;
};

struct Events
{
	HANDLE ConfirmEvent = NULL;
	HANDLE StartEvent = NULL;
	HANDLE StopEvent = NULL;
	HANDLE QuitEvent = NULL;
	HANDLE MessageEvent = NULL;
	HANDLE Mutex = NULL;
};

Events events;
vector<HANDLE> CloseEvent;
vector<HANDLE> SessionEvent;
vector<HANDLE> MessageEvent;
const BYTE SIZE_DESCRIPTION = sizeof(Description);

//возвращаем номер 
Description getDescription() {
	HANDLE hFileMap = CreateFileMapping(NULL, NULL, PAGE_READWRITE, 0, SIZE_DESCRIPTION, "messageFile");
	BYTE* pBuf = (BYTE*)MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, SIZE_DESCRIPTION);

	Description descr;
	memcpy(&descr, pBuf, SIZE_DESCRIPTION);

	UnmapViewOfFile(pBuf);
	CloseHandle(hFileMap);
	return descr;
}

HANDLE getCloseEvent(int threadNumber)
{
	return CloseEvent[threadNumber];
}

HANDLE getSessionEvent(int threadNumber)
{
	return SessionEvent[threadNumber];
}

// функции для диалогового окна
extern "C" 
{
	// работа с событиями
	__declspec(dllexport)// создаём дескрипторы
		void _stdcall createEvents()
	{
		events.ConfirmEvent = CreateEvent(NULL, FALSE, FALSE, "Confirm_Event");
		events.StartEvent = CreateEvent(NULL, FALSE, FALSE, "Start_Event");
		events.StopEvent = CreateEvent(NULL, FALSE, FALSE, "Stop_Event");
		events.QuitEvent = CreateEvent(NULL, FALSE, FALSE, "Quit_Event");
		events.MessageEvent = CreateEvent(NULL, FALSE, FALSE, "Message_Event");
		events.Mutex = CreateMutex(NULL, FALSE, "Mutex");
	}

	__declspec(dllexport)// ожидаем подтверждения события
		void _stdcall waitEventConfirm()
	{
		WaitForSingleObject(events.ConfirmEvent, INFINITE);
	}

	__declspec(dllexport)// инициируем создание нового потока
		void _stdcall setEventStart()
	{
		SetEvent(events.StartEvent);
	}

	__declspec(dllexport)// инициируем остановку последнего потока
		void _stdcall setEventStop()
	{
		SetEvent(events.StopEvent);
	}

	__declspec(dllexport)// инициируем выход из консоли
		void _stdcall setEventQuit()
	{
		SetEvent(events.QuitEvent);
	}

	__declspec(dllexport)// инициируем отправку сообщения
		void _stdcall setEventMessage()
	{
		SetEvent(events.MessageEvent);
	}

	// перенос сообщения из диалогового окна в виртуальное адресное пространство
	__declspec(dllexport)// переносим сообещение в виртуальное адресное пространство, с дальнейшим выводом в файл и консоль
		void __stdcall createMessage(char* message, int threadNumber)
	{
		Description descr;// создаём структуру с ID потока и размером его сообщения +1 (завершающий бит)
		descr.size = strlen(message) + 1;
		descr.threadNumber = threadNumber;

		HANDLE FileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SIZE_DESCRIPTION + descr.size, "messageFile");
		// Создаём файловое отображение, только что созданного файла, с правами чтения и записи, размер файла для отображения = 0,
		// размер отображаемой области равен размеру сообщения и памяти на хранение размера сообщения и номера его потока,
		// с именем messageFile. С помощью имени другие потоки смогут открыть то же самое пространство
		BYTE* pBuf = (BYTE*)MapViewOfFile(FileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, SIZE_DESCRIPTION + descr.size);
		// отображаем созданное отображение в виртуальное адресное пространство нашего процесса. Дескриптор файлового отображения FileMap,
		// ставим модификаторы на чтение и запись, указываем смещение от начала файла = 0, а размер пространства равен размеру сообщения +
		// + размер памяти на хранение размера сообщения и номера его потока

		memcpy(pBuf, &descr, SIZE_DESCRIPTION); // копируем содержимое структуры в виртуальное пространство, указав размер структуры
		memcpy(pBuf + SIZE_DESCRIPTION, message, descr.size);//аналогично для сообщения со сдвигом в вируальном пространстве 

		setEventMessage();//иницируем отправку сообщений
		waitEventConfirm();// ожидаем окончания вывода сообщений (подтверждения)

		UnmapViewOfFile(pBuf);// отменяем отображение файла из памяти
		CloseHandle(FileMap);// закрываем дескриптор файла
	}
}

// функции для консольного окна
extern "C"
{
	__declspec(dllexport)//считывание сообщение из виртуального адресного пространства
		char* __stdcall getMessage(int& threadNumber)
	{
		Description descr = getDescription();
		threadNumber = descr.threadNumber;

		HANDLE hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SIZE_DESCRIPTION + descr.size, "messageFile");
		BYTE* pBuf = (BYTE*)MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, SIZE_DESCRIPTION + descr.size);

		char* message = new char[descr.size];
		memcpy(message, pBuf + SIZE_DESCRIPTION, descr.size);

		UnmapViewOfFile(pBuf);
		CloseHandle(hFileMap);

		return message;
	}


	//работа с событиями
	__declspec(dllexport)// присваиваем и возвращаем номер, соответствующий тому или иному событию (для меню)
		int _stdcall getCurrentEvent()
	{
		HANDLE hEvents[4];
		hEvents[0] = events.StartEvent;
		hEvents[1] = events.StopEvent;
		hEvents[2] = events.QuitEvent;
		hEvents[3] = events.MessageEvent;

		int eventNumber = WaitForMultipleObjects(4, hEvents, FALSE, INFINITE) - WAIT_OBJECT_0;

		return eventNumber;
	}

	__declspec(dllexport)// подтверждаем событие
		void _stdcall setEventConfirm()
	{
		SetEvent(events.ConfirmEvent);
	}


	// удаление дескрипторов сообщений, потоков, событий
	__declspec(dllexport)// удаляяем вектор сообщений
		void _stdcall deleteMessageEvents()
	{
		for (auto event : MessageEvent)
		{
			CloseHandle(event);
		}
	}

	__declspec(dllexport)// закрытие потоков и очистка вектора потоков
		void _stdcall deleteCloseEvents()
	{
		for (auto event : CloseEvent)
		{
			SetEvent(event);
			CloseHandle(event);
		}
	}
	
	__declspec(dllexport)// закрытие событий потока
		void _stdcall deleteSessionEvents()
	{
		for (auto event : SessionEvent)
		{
			CloseHandle(event);
		}
	}

	__declspec(dllexport)// удаляем дескрипторы
		void _stdcall deleteEvents()
	{
		CloseHandle(events.ConfirmEvent);
		CloseHandle(events.StartEvent);
		CloseHandle(events.StopEvent);
		CloseHandle(events.QuitEvent);
		CloseHandle(events.MessageEvent);
		CloseHandle(events.Mutex);
	}

	// работа с векторами 
	__declspec(dllexport)// добавляем новый поток
		void _stdcall addCloseEvent()
	{
		CloseEvent.push_back(CreateEvent(NULL, FALSE, FALSE, NULL));
	}

	__declspec(dllexport)// добавляем новый поток
		void _stdcall addSessionEvent()
	{
		SessionEvent.push_back(CreateEvent(NULL, FALSE, FALSE, NULL));
	}

	__declspec(dllexport)// добавляем новое сообщение
		void _stdcall addMessageEvent()
	{
		MessageEvent.push_back(CreateEvent(NULL, FALSE, FALSE, NULL));
	}

	__declspec(dllexport)// Удаляем последний поток
		void _stdcall popBackCloseEvent()
	{
		CloseEvent.pop_back();
	}

	__declspec(dllexport)// делаем проверку на пустоту вектора потоков
		boolean _stdcall closeEventIsEmpty()
	{
		return !CloseEvent.empty();
	}

	__declspec(dllexport)// возвращаем количество потоков
		int _stdcall getCloseEventSize()
	{
		return CloseEvent.size();
	}

	__declspec(dllexport)// удаляем поток с номером threadNumber
		void _stdcall closeClose(int threadNumber)
	{
		CloseHandle(getCloseEvent(threadNumber));
	}

	__declspec(dllexport)// удаляем поток с номером threadNumber
		void _stdcall closeSession(int threadNumber)
	{
		CloseHandle(getSessionEvent(threadNumber));
	}

	// работа с сессией
	__declspec(dllexport)// ожидаем сессию
		void _stdcall waitSession(int threadNumber)
	{
		WaitForSingleObject(getSessionEvent(threadNumber), INFINITE);
	}

	__declspec(dllexport)// ожидаем сессию
		void _stdcall setSession(int threadNumber)
	{
		SetEvent(getSessionEvent(threadNumber));
	}

	__declspec(dllexport)// ожидаем сессию
		void _stdcall resetSession(int threadNumber)
	{
		ResetEvent(getSessionEvent(threadNumber));
	}

	// мьютикс
	__declspec(dllexport)// освобождаем мьютикс
		void _stdcall releaseMutex()
	{
		ReleaseMutex(events.Mutex);
	}

	__declspec(dllexport)// ожидаем мьютикс
		void _stdcall waitMutex()
	{
		WaitForSingleObject(events.Mutex, INFINITE);
	}

}


// CStolerAA2107LibraryMFCApp
BEGIN_MESSAGE_MAP(CStolerAA2107LibraryMFCApp, CWinApp)
END_MESSAGE_MAP()

// Создание CStolerAA2107LibraryMFCApp
CStolerAA2107LibraryMFCApp::CStolerAA2107LibraryMFCApp()
{
	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
	createEvents();
}

// Единственный объект CStolerAA2107LibraryMFCApp
CStolerAA2107LibraryMFCApp theApp;
// Инициализация CStolerAA2107LibraryMFCApp
BOOL CStolerAA2107LibraryMFCApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}