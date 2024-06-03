#include "pch.h"
#include "framework.h"
#include "Stoler_DLL.h"
#include "string"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL CStolerDLLApp::InitInstance()
{
	
	return 0;
}

struct header {
	int id;
	int commandId;
	int size;
};

// 1 - добавить новый поток
// 2 - стоп потока
// 3 - отправить сообщение
// 4 - получить обновленный список

extern "C" {

	_declspec (dllexport) bool Connect() {
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		AfxSocketInit();
		s.Create();
		if (s.Connect("127.0.0.1", 1111)) {
			return true;
		}
		return false;
	}


	_declspec (dllexport) char* sendCommand(int id, int commandId, char* message, int size = 0) {
		char* result = nullptr;
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		header h{id, commandId, size};
		s.Send(&h, sizeof(header));
		if (size != 0) {
			s.Send(message, size);
		}
		
		if (commandId == 4) {
			int sResponse;
			s.Receive(&sResponse, sizeof(int));
			if (sResponse == 0) {
				return result;
			}
			result = new char[sResponse];
			s.Receive(result, sResponse);
			
		} 
		return result;
	}

	_declspec (dllexport) void Disconnect() {
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		s.Close();
	}
}

