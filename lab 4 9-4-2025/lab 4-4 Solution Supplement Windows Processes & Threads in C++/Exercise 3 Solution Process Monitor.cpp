#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <iomanip>
struct ProcessEntry {
DWORD processID;
std::wstring processName;
DWORD parentProcessID;
DWORD threadCount;
};
int main() {
HANDLE hProcessSnap;
PROCESSENTRY32W pe32;
std::vector<ProcessEntry> processes;
// Take a snapshot of all processes
hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
if (hProcessSnap == INVALID_HANDLE_VALUE) {
std::wcerr << L"CreateToolhelp32Snapshot failed. Error: " << GetLastError() << std::endl;
return 1;
}
// Set the size ofthe structure before using it
pe32.dwSize = sizeof(PROCESSENTRY32W);
// Get the first process
if (!Process32FirstW(hProcessSnap, &pe32)) {
std::wcerr << L"Process32First failed. Error: " << GetLastError() << std::endl;
CloseHandle(hProcessSnap);
return 1;
}
// Walk through the process list
do {
ProcessEntry entry;
entry.processID = pe32.th32ProcessID;
entry.processName = pe32.szExeFile;
entry.parentProcessID = pe32.th32ParentProcessID;
entry.threadCount = pe32.cntThreads;
processes.push_back(entry);
} while (Process32NextW(hProcessSnap, &pe32));
CloseHandle(hProcessSnap);

Part 2: Thread Exercises Solutions
Exercise 4 Solution: Producer-Consumer
// Display process information
std::wcout << L"Running Processes:" << std::endl;
std::wcout << std::left << std::setw(8) << L"PID"
<< std::setw(8) << L"PPID"
<< std::setw(8) << L"Threads"
<< L"Process Name" << std::endl;
std::wcout << std::wstring(60, L'-') << std::endl;
for(const auto& proc : processes) {
std::wcout << std::left << std::setw(8) << proc.processID
<< std::setw(8) << proc.parentProcessID
<< std::setw(8) << proc.threadCount
<< proc.processName << std::endl;
}
std::wcout << L"\nTotal processes: " << processes.size() << std::endl;
return 0;
}