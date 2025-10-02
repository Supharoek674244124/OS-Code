// CreateProcess example (MinGW / Win32)
// Launch any program entered by user
#include <windows.h>
#include <stdio.h>

int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char commandLine[1024]; // buffer for user input

    printf("Enter program to run (e.g., notepad.exe or calc.exe):\n> ");

    if (!fgets(commandLine, sizeof(commandLine), stdin))
    {
        printf("Error reading input\n");
        return 1;
    }

    // Remove newline at end (fgets leaves it)
    size_t len = strlen(commandLine);
    if (len > 0 && (commandLine[len - 1] == '\n' || commandLine[len - 1] == '\r'))
        commandLine[len - 1] = '\0';

    printf("Parent PID: %lu\n", GetCurrentProcessId());

    BOOL ok = CreateProcessA(
        NULL,        // Application name (NULL -> use commandLine)
        commandLine, // Command line (must be mutable buffer)
        NULL, NULL,  // Security attributes
        FALSE,       // Inherit handles
        0,           // Creation flags
        NULL,        // Environment
        NULL,        // Current directory
        &si, &pi);

    if (!ok)
    {
        DWORD e = GetLastError();
        printf("CreateProcess failed (error %lu)\n", e);
        return 1;
    }

    printf("Child PID: %lu\n", pi.dwProcessId);

    // Wait for the child to finish
    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
