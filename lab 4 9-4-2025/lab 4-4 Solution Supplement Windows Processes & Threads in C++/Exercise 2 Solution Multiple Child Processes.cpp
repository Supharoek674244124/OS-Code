#include <iostream>
#include <windows.h>
#include <vector>
struct ProcessInfo
{
    PROCESS_INFORMATION pi;
    std::wstring command;
};
int main()
{
    const int NUM_PROCESSES = 3;
    std::vector<ProcessInfo> processes(NUM_PROCESSES);
    std::vector<HANDLE> processHandles(NUM_PROCESSES);
    // Commands to execute
    std::wstring commands[] = {
        L"cmd.exe /c echo Process 1 && timeout 2",
        L"cmd.exe /c echo Process 2 && timeout 3",
        L"cmd.exe /c echo Process 3 && timeout 1"};
    std::wcout << L"Creating " << NUM_PROCESSES << L" child processes..." << std::endl;
    // Create processes
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        STARTUPINFO si;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&processes[i].pi, sizeof(processes[i].pi));
        processes[i].command = commands[i];
        if (CreateProcess(
                NULL,
                const_cast<LPWSTR>(processes[i].command.c_str()),
                NULL, NULL, FALSE, 0, NULL, NULL, &si, &processes[i].pi))
        {
            std::wcout << L"Created process " << (i + 1) << L" with PID: "
                       << processes[i].pi.dwProcessId << std::endl;
            processHandles[i] = processes[i].pi.hProcess;
        }
        else
        {
            std::wcerr << L"Failed to create process " << (i + 1) << std::endl;
            processHandles[i] = NULL;
        }
    }

                              std::wcout
                          << L"Waiting for all processes to complete..."
                          << std::endl;
    // Wait for all processes to complete
    WaitForMultipleObjects(NUM_PROCESSES, processHandles.data(), TRUE, INFINITE);
    std::wcout << L"All processes completed!" << std::endl;
    // Clean up handles
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        if (processHandles[i] != NULL)
        {
            CloseHandle(processes[i].pi.hProcess);
            CloseHandle(processes[i].pi.hThread);
        }
    }
    return 0;
}