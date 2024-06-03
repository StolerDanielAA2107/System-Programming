#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы
#include <vector>
#include <iostream>
#include <fstream>

class CStolerAA2107LibraryMFCApp : public CWinApp
{
public:
	CStolerAA2107LibraryMFCApp();

// Переопределение
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

