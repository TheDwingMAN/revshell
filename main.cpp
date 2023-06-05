#include <iostream>
#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#include <string>
#include <sstream>


HANDLE OutputPipeReadEnd = NULL;
HANDLE OutputPipeWriteEnd = NULL;
HANDLE InputPipeReadEnd = NULL;
HANDLE InputPipeWriteEnd = NULL;

STARTUPINFO si;
PROCESS_INFORMATION pi;

using namespace std;

// functions signattroes
boolean CreatPipes(SECURITY_ATTRIBUTES sa);
void WriteToPip();
void ReadFromPip();
boolean CreatePowerShell();
boolean IsInReg();
boolean AddToReg();


boolean CreatePowerShell() {

	wstring processName = L"powershell.exe";
	//wcout << L"Enter the name of the process to run: ";
	//getline(wcin, processName);
	wstring Dircoty = L"C:\\Users\\ronen\\Documents\\school";

	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.hStdError = OutputPipeWriteEnd;
	si.hStdOutput = OutputPipeWriteEnd;
	si.hStdInput = InputPipeReadEnd;
	si.dwFlags |= STARTF_USESTDHANDLES;

	// Start the child process. 
	if (!CreateProcess(NULL,   // No module name (use command line)
		const_cast<LPWSTR>(processName.c_str()),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		TRUE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		const_cast<LPWSTR>(Dircoty.c_str()),           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return false;
	}
	return true;
}

boolean CreatePipesForInProcess() {
	SECURITY_ATTRIBUTES sa;
	// make the child to get the pipes too i mean inheretnce wwowo
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	// pip A --> child read parnt write
	if (!CreatePipe(&OutputPipeReadEnd, &OutputPipeWriteEnd, &sa, 0)) {
		cout << "pip A isnt working" << endl;
		return false;
	}
	//pip B --> child write parant read
	if (!CreatePipe(&InputPipeReadEnd, &InputPipeWriteEnd, &sa, 0)) {
		cout << "pip B isnt working idk why my man" << endl;
		return false;
	}
	return true;

}

boolean ReadOutputPowershell() {
	char buffer[4096] = { 0 };
	DWORD bytesRead = 0;
	//char output[4096] = { 0 };
	//size_t outputLength = 0;
	DWORD avil = 0;

	Sleep(300);

	while ((PeekNamedPipe(OutputPipeReadEnd, NULL, 0, NULL, &avil, NULL)) && avil > 0) {
		while (avil != 0) {
			if (!ReadFile(OutputPipeReadEnd, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
				cout << "read file isnt working" << endl;
				return false;
			}
			buffer[bytesRead] = '\0';
			cout << buffer;
			avil = avil - bytesRead;
		}
	}
	if (avil == 0) {
		return true;
	}

	return false;
}

boolean WriteToPoweShell(string Commend, DWORD* NumberOfBytesWritten) {
	bool isSucces = false;
	 
	Commend += "\n";
	isSucces = WriteFile(

		InputPipeWriteEnd,     // use pipe A

		Commend.c_str(),

		strlen(Commend.c_str()),

		NumberOfBytesWritten,

		NULL
	);
	if (!isSucces) {
		cout << "shit bro this isnt working we are fucked" + GetLastError() << endl;
		return false;
	}
	return true;
}

void _tmain(int argc, TCHAR* argv[])
{

	//setup info
	DWORD NumberOfBytesWritten;
	ZeroMemory(&NumberOfBytesWritten, sizeof(DWORD));
	//end of setup info


	//create the pipes
	if (!CreatePipesForInProcess()) {
		cout << "faild to creat pipes" + GetLastError() << endl;
		return;
	}
	else
	{
		cout << "pipes are working" << endl;
	}
	// end creation of pies


	//creating the child process
	if (!CreatePowerShell()) {
		cout << "shit this isnt working" << endl;
		return;
	}
	else {
		cout << "powershell proccess is up" << endl;
	}
	cout << "enter the commend or type exit! to quit" << endl;
	//end of the child process creation


	//loop for I/O
	string UserInput;
	if (!ReadOutputPowershell()) {
		cout << "reading isnt workign" << endl;
	}
	for (;;) {
		getline(cin, UserInput);
		if (UserInput == "exit!") {
			cout << "beyyyyyy my man" << endl;
			break;
		}
		cout << "sending to the powershell " + UserInput << endl;
		if (!WriteToPoweShell(UserInput, &NumberOfBytesWritten)) {
			cout << "problem sending" + GetLastError() << endl;
			break;
		}
		if (!ReadOutputPowershell()) {
			cout << "reading isnt workign" << endl;
			break;
		}
	}
	cout << "out loop" << endl;


	//closing precudersces

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	// closing the pipes to really close the porcess
	CloseHandle(InputPipeReadEnd);
	CloseHandle(OutputPipeWriteEnd);
}


