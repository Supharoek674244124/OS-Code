#include <windows.h>
#include <stdio.h>
int main() {
 STARTUPINFO si1, si2;
 PROCESS_INFORMATION pi1, pi2;
 ZeroMemory(&si1, sizeof(si1));
 si1.cb = sizeof(si1);
 ZeroMemory(&pi1, sizeof(pi1));
 ZeroMemory(&si2, sizeof(si2));
 si2.cb = sizeof(si2);
 ZeroMemory(&pi2, sizeof(pi2));
 printf("Parent Process ID: %lu\n", GetCurrentProcessId());
 if (!CreateProcess("C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, 0,
NULL, NULL, &si1, &pi1)) {
 printf("First CreateProcess failed (%d).\n", GetLastError());
 return 1;
 }
 printf("First Child Process ID: %lu\n", pi1.dwProcessId);
 if (!CreateProcess("C:\\Windows\\System32\\mspaint.exe", NULL, NULL, NULL, FALSE, 0,
NULL, NULL, &si2, &pi2)) {
 printf("Second CreateProcess failed (%d).\n", GetLastError());
 return 1;
 }
 printf("Second Child Process ID: %lu\n", pi2.dwProcessId);
 WaitForSingleObject(pi1.hProcess, INFINITE);
 WaitForSingleObject(pi2.hProcess, INFINITE);
 printf("Both child processes finished.\n");
 CloseHandle(pi1.hProcess);
 CloseHandle(pi1.hThread);
 CloseHandle(pi2.hProcess);
 CloseHandle(pi2.hThread);
 return 0;
}
