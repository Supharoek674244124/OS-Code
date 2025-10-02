#include <iostream>
#include <windows.h>
// Thread function
DWORD WINAPI ThreadFunction(LPVOID lpParam) {
int threadId = *(int*)lpParam;
for(int i = 0; i < 5; i++) {
std::cout << "Thread " << threadId << " - Count: " << i << std::endl;
Sleep(1000); // Sleep for 1 second
}
return 0;
}
int main() {
const int NUM_THREADS = 3;
HANDLE threads[NUM_THREADS];
int threadIds[NUM_THREADS];
// Create threads
for(int i = 0; i < NUM_THREADS; i++) {
threadIds[i] = i + 1;
threads[i] = CreateThread(
NULL, // Security attributes
0, // Stack size
ThreadFunction, // Thread function
&threadIds[i], // Parameter
0, // Creation flags
NULL // Thread ID
);
if (threads[i] == NULL) {
std::cerr << "Failed to create thread " << i << std::endl;
return 1;
}
}
// Wait for allthreads to complete
WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);
// Close thread handles
for(int i = 0; i < NUM_THREADS; i++) {
CloseHandle(threads[i]);
}
std::cout << "All threads completed!" << std::endl;
return 0;
}