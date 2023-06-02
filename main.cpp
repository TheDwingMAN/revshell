#include <iostream>
#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#include <string>
#include <sstream>


HANDLE ChildWritePipEnd = NULL;
HANDLE ChildReadPipEnd = NULL;
HANDLE ParnatWritePipEnd = NULL;
HANDLE ParnatReadPipEnd = NULL;

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

    PROCESS_INFORMATION pi;
    STARTUPINFO si;


    wstring processName = L"powershell.exe";
    //wcout << L"Enter the name of the process to run: ";
    //getline(wcin, processName);
    wstring Dircoty = L"C:\\Users\\ronen\\Documents\\school";

    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.hStdError = ParnatWritePipEnd;
    si.hStdOutput = ParnatWritePipEnd;
    si.hStdInput = ChildReadPipEnd;
    si.dwFlags |= STARTF_USESTDHANDLES;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    


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
    if (!CreatePipe(&ChildReadPipEnd, &ParnatWritePipEnd, &sa, 0)) {
        cout << "pip A isnt working" << endl; 
        return false;
    }
    //pip B --> child write parant read
    if (!CreatePipe(&ChildWritePipEnd, &ParnatReadPipEnd, &sa, 0)) {
        cout << "pip B isnt working idk why my man" << endl;
        return false;
    }
    return true;

}
boolean ReadOutputPowershell() {
    char buffer[4096];
    DWORD bytesRead;
    char output[4096] = { 0 };
    size_t outputLength = 0;
    // main loop for reading output 
    for (;;) {
        ZeroMemory(buffer, sizeof(buffer));

        if (!ReadFile(ChildWritePipEnd, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            cout << "cant really read from the pipe opsi" + GetLastError() << endl;
            return false;
        }
        if (bytesRead == 0)
        {
            // End of file reached
            cout << "all the output" << endl;
            return true;
        }
        buffer[bytesRead] = '\0';
        // Append the output to the accumulated output string
        if (outputLength + bytesRead < sizeof(output))
        {
            strcat_s(output, buffer);
            outputLength += bytesRead;
        }
        // Check if the output contains a complete command result
        char* commandResult = strstr(output, "\n");
        while (commandResult != NULL)
        {
            // Null-terminate the command result
            *commandResult = '\0';

            // Display the command result on the console
            printf("%s\n", output);

            // Move the remaining output to the beginning
            memmove(output, commandResult + 1, outputLength - (commandResult - output));

            // Update the output length
            outputLength -= (commandResult - output + 1);

            // Find the next command result
            commandResult = strstr(output, "\n");
        }

    }
}

boolean WriteToPoweShell(string Commend,DWORD* NumberOfBytesWritten) {
        bool isSucces = false;

        isSucces = WriteFile(

        ParnatWritePipEnd,     // use pipe A

        Commend.c_str(),

        strlen(Commend.c_str()),

        NumberOfBytesWritten,

        NULL
    );
        if (!isSucces) { cout << "shit bro this isnt working we are fucked" + GetLastError() << endl;
        return false;
        }
        return true;
}

void _tmain(int argc, TCHAR* argv[])
{

    //setup info
    DWORD NumberOfBytesWritten;
    ZeroMemory(&NumberOfBytesWritten, sizeof(DWORD));

    SECURITY_ATTRIBUTES sa; 
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
    for (;;) {
        getline(cin, UserInput);
        if (UserInput == "exit!") {
            cout << "beyyyyyy my man" << endl;
            break;
        }
        cout << "sending to the powershell " + UserInput << endl;
        if (!WriteToPoweShell(UserInput, &NumberOfBytesWritten)) {
            cout << "problem sending" +GetLastError()<< endl;
            break;
      }
        if (!ReadOutputPowershell()) {
            cout << "reading isnt workign" << endl;
            break;
        }
    }


    //closing precudersces
    
    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // closing the pipes to really close the porcess
    CloseHandle(ParnatWritePipEnd);
    CloseHandle(ChildReadPipEnd);
}


