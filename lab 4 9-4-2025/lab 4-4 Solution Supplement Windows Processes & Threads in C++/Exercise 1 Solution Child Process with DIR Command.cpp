#include <iostream>
#include <windows.h>
#include <string>
int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    // Initialize structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    // Create process to execute dir command
    std::wstring cmdLine = L"cmd.exe /c dir";
    if (CreateProcess(
            NULL,                                // No module name
            const_cast<LPWSTR>(cmdLine.c_str()), // Command line
            NULL,                                // Process security attributes
            NULL,                                // Thread security attributes
            FALSE,                               // Don't inherit handles
            CREATE_NO_WINDOW,                    // Creation flags - no console window
            NULL,                                // Use parent's environment
            NULL,                                // Use parent's current directory
            &si,                                 // Startup info
            &pi                                  // Process information
            ))
    {
        std::wcout << L"DIR command process created successfully!" << std::endl;
        std::wcout << L"Process ID: " << pi.dwProcessId << std::endl;
        // Wait forthe process to finish
        WaitForSingleObject(pi.hProcess, INFINITE);
        // Get exit code
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        std::wcout << L"Process finished with exit code: " << exitCode << std::endl;
        // Close handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        std::wcerr << L"Failed to create process. Error: " << GetLastError() << std::endl;
    }

    return 0;
}