// Server process
#include <iostream>
#include <windows.h>
#include <string>
int main() {
const wchar_t* pipeName = L"\\\\.\\pipe\\MyNamedPipe";
HANDLE hPipe;
std::wcout << L"Creating named pipe server..." << std::endl;
// Create named pipe
hPipe = CreateNamedPipe(
pipeName, // Pipe name
PIPE_ACCESS_DUPLEX, // Read/write access
PIPE_TYPE_MESSAGE | // Message type pipe
PIPE_READMODE_MESSAGE | // Message-read mode
PIPE_WAIT, // Blocking mode
PIPE_UNLIMITED_INSTANCES, // Max instances
1024, // Out buffer size
1024, // In buffer size
0, // Timeout
NULL // Security attributes
);
if (hPipe == INVALID_HANDLE_VALUE) {
std::wcerr << L"CreateNamedPipe failed: " << GetLastError() << std::endl;
return 1;
}
std::wcout << L"Waiting for client connection..." << std::endl;
// Wait for client connection
if (ConnectNamedPipe(hPipe, NULL) || GetLastError() == ERROR_PIPE_CONNECTED) {
std::wcout << L"Client connected!" << std::endl;
// Communication loop
char buffer[1024];
DWORD bytesRead;
while (true) {
// Read from client
if (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
buffer[bytesRead] = '\0';
std::cout << "Received: " << buffer << std::endl;

if (strcmp(buffer, "quit") == 0) break;
// Echo back to client
std::string response = "Echo: " + std::string(buffer);
DWORD bytesWritten;
WriteFile(hPipe,response.c_str(),response.length(), &bytesWritten, NULL);
} else {
std::wcerr << L"ReadFile failed: " << GetLastError() << std::endl;
break;
}
}
}
CloseHandle(hPipe);
std::wcout << L"Server shutting down." << std::endl;
return 0;
}
// Client process
#include <iostream>
#include <windows.h>
#include <string>
int main() {
const wchar_t* pipeName = L"\\\\.\\pipe\\MyNamedPipe";
HANDLE hPipe;
std::wcout << L"Connecting to named pipe..." << std::endl;
// Connect to named pipe
hPipe = CreateFile(
pipeName, // Pipe name
GENERIC_READ | GENERIC_WRITE,// Access mode
0, // Share mode
NULL, // Security attributes
OPEN_EXISTING, // Creation disposition
0, // Flags and attributes
NULL // Template file handle
);
if (hPipe == INVALID_HANDLE_VALUE) {
std::wcerr << L"CreateFile failed: " << GetLastError() << std::endl;
return 1;
}
std::wcout << L"Connected to server!" << std::endl;

Question 11 Solution: Performance Comparison
// Communication loop
std::string message;
char buffer[1024];
DWORD bytesWritten, bytesRead;
while (true) {
std::cout << "Enter message (or'quit' to exit): ";
std::getline(std::cin, message);
// Send to server
if (WriteFile(hPipe, message.c_str(), message.length(), &bytesWritten, NULL)) {
if (message == "quit") break;
// Read response
if (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
buffer[bytesRead] = '\0';
std::cout << "Serverresponse: " << buffer << std::endl;
}
} else {
std::wcerr << L"WriteFile failed: " << GetLastError() << std::endl;
break;
}
}
CloseHandle(hPipe);
std::wcout << L"Client disconnected." << std::endl;
return 0;
}