#include <iostream>
#include <windows.h>
int main() {
DWORD processId = GetCurrentProcessId();
HANDLE processHandle = GetCurrentProcess();
std::cout << "Current Process Information:" << std::endl;
std::cout << "Process ID: " << processId << std::endl;
std::cout << "Process Handle: " << processHandle << std::endl;
// Get process creation time
FILETIME creationTime, exitTime, kernelTime, userTime;
if (GetProcessTimes(processHandle, &creationTime, &exitTime, &kernelTime, &userTime)) {
std::cout << "Process created successfully" << std::endl;
}
return 0;
}