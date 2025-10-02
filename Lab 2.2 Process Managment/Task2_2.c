#include <windows.h>
#include <stdio.h>
int main() {
 STARTUPINFO si;
 PROCESS_INFORMATION pi;
 ZeroMemory(&si, sizeof(si));
 si.cb = sizeof(si);
 ZeroMemory(&pi, sizeof(pi));
 printf("Parent Process ID: %lu\n", GetCurrentProcessId());
 if (!CreateProcess(
 "C:\\Windows\\System32\\notepad.exe",
 NULL,
 NULL,
 NULL,
 FALSE,
 0,
 NULL,
 NULL,
 &si,
 &pi)
 ) {
 printf("CreateProcess failed (%d).\n", GetLastError());
 return 1;
 }
 printf("Child Process ID: %lu\n", pi.dwProcessId);
 WaitForSingleObject(pi.hProcess, INFINITE);
 printf("Child process finished.\n");
 CloseHandle(pi.hProcess);
 CloseHandle(pi.hThread);
 return 0;
}

