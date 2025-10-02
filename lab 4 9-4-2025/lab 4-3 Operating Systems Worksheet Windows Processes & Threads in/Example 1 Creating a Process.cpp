#include <iostream>
#include <windows.h>
int main() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
// Initialize structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
// Create process (notepad.exe)
if (CreateProcess(
    L"C:\\Windows\\System32\\notepad.exe", // Application name
    NULL, // Command line
    NULL, // Process security attributes
    NULL, // Thread security attributes
    FALSE, // Inherit handles
    0, // Creation flags
    NULL, // Environment
    NULL, // Current directory
    &si, // Startup info
    &pi // Process information
)) {
    std::cout << "Process created successfully!" << std::endl;
    std::cout << "Process ID: " << pi.dwProcessId << std::endl;
// Wait for process to finish
WaitForSingleObject(pi.hProcess, INFINITE);
// Close handles
CloseHandle(pi.hProcess);
CloseHandle(pi.hThread);
} else {
std::cerr << "Failed to create process. Error: " << GetLastError() << std::endl;
}
return 0;
}