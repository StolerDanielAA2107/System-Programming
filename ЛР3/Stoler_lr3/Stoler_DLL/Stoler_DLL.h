// Stoler_DLL.h : main header file for the Mustafaev_DLL DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CStolerDLLApp
// See Stoler_DLL.cpp for the implementation of this class
//

class CStolerDLLApp : public CWinApp
{
public:
	CStolerDLLApp();
	
// Overrides
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

CSocket s;