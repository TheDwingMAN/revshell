//#include <iostream>
//#include <windows.h> 
//#include <tchar.h>
//#include <stdio.h> 
//#include <strsafe.h>
//#include <string>
//#include <sstream>
//
//using namespace std;
//
//HANDLE OutputPipeReadEnd = NULL;
//HANDLE OutputPipeWriteEnd = NULL;
//HANDLE InputPipeReadEnd = NULL;
//HANDLE InputPipeWriteEnd = NULL;
//
//boolean CreatePowershell(_Out_ PROCESS_INFORMATION* pi) {
//
//	STARTUPINFO si = {0};
//
//	wstring processName = L"powershell.exe";
//	wstring Dircoty = L"C:\\Users\\ronen\\Documents\\school";
//
//	ZeroMemory(pi, sizeof(PROCESS_INFORMATION));
//	si.cb = sizeof(STARTUPINFO);
//	si.hStdError = OutputPipeWriteEnd;
//	si.hStdOutput = OutputPipeWriteEnd;
//	si.hStdInput = InputPipeReadEnd;
//	si.dwFlags |= STARTF_USESTDHANDLES;
//
//	
//
//	return true;
//}