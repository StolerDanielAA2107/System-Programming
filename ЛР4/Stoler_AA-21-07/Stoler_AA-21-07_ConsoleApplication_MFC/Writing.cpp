#pragma once
#include "pch.h"
#include "Writing.h"

void SafeFile(string fName, string& message)
{
	waitMutex();
	//cs.Lock();
	ofstream fout(fName);
	fout << message;
	fout.close();
	releaseMutex();
	//cs.Unlock();
}