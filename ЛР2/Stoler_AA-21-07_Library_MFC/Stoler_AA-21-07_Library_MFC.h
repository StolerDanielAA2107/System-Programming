// Stoler_AA-21-07_Library_MFC.h: основной файл заголовка для библиотеки DLL Stoler_AA-21-07_Library_MFC
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы
#include <vector>
#include <iostream>
#include <fstream>


// CStolerAA2107LibraryMFCApp
// Реализацию этого класса см. в файле Stoler_AA-21-07_Library_MFC.cpp
//

class CStolerAA2107LibraryMFCApp : public CWinApp
{
public:
	CStolerAA2107LibraryMFCApp();

// Переопределение
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
