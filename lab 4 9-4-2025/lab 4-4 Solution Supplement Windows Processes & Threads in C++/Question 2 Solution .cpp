//Question 2 Solution
#include <iostream>
#include <windows.h>
int main() {
STARTUPINFO si;
PROCESS_INFORMATION pi;
// Initialize structures
ZeroMemory(&si, sizeof(si));
si.cb = sizeof(si);
ZeroMemory(&pi, sizeof(pi));
std::wcout << L"Parent Process ID: " << GetCurrentProcessId() << std::endl;
// Create child process to run ping
std::wstring cmdLine = L"ping google.com";
if (CreateProcess(
NULL, // No module name
const_cast<LPWSTR>(cmdLine.c_str()), // Command line
NULL, // Process security attributes
NULL, // Thread security attributes
FALSE, // Don't inherit handles
0, // Default creation flags
NULL, // Use parent's environment
NULL, // Use parent's current directory
&si, // Startup info
&pi // Process information
)) {
std::wcout << L"Child process created successfully!" << std::endl;
std::wcout << L"Child Process ID: " << pi.dwProcessId << std::endl;
std::wcout << L"Child Thread ID: " << pi.dwThreadId << std::endl;
// Monitor process status
std::wcout << L"Monitoring child process..." << std::endl;
DWORD waitResult = WaitForSingleObject(pi.hProcess, INFINITE);
switch (waitResult) {
case WAIT_OBJECT_0:
std::wcout << L"Child process completed successfully." << std::endl;
break;
case WAIT_TIMEOUT:
std::wcout << L"Child process timed out." << std::endl;
break;
case WAIT_FAILED:

Question 3 Solution:
Significance of Process Handles:
Handles are references to kernel objects
They provide controlled access to system resources
Each handle has associated access rights
Handles are process-specific and must be explicitly closed
Failing to close handles leads to resource leaks
System has limited handle capacity - leaks can cause system instability
Section B: Thread Management Solutions
Question 4 Solution:
Processes vs Threads Comparison:
std::wcout << L"Wait failed. Error: " << GetLastError() << std::endl;
break;
}
// Get exit code
DWORD exitCode;
if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
std::wcout << L"Child process exit code: " << exitCode << std::endl;
}
// Close handles
CloseHandle(pi.hProcess);
CloseHandle(pi.hThread);
} else {
std::wcerr << L"Failed to create child process. Error: " << GetLastError() << std::endl;
}
return 0;
}