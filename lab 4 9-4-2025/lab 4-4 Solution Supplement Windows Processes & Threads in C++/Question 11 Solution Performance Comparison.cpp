#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <windows.h>
// Test function forthreads
void simpleTask(int iterations) {
volatile int sum = 0;
for(int i = 0; i < iterations; i++) {
sum += i;
}
}
// Windows APIthread function
DWORD WINAPI winApiTask(LPVOID lpParam) {
int iterations = *(int*)lpParam;
simpleTask(iterations);
return 0;
}
// Benchmark Windows APIthreads
double benchmarkWinApiThreads(int numThreads, int iterations) {
std::vector<HANDLE> threads(numThreads);
auto start = std::chrono::high_resolution_clock::now();
// Create threads
for(int i = 0; i < numThreads; i++) {
threads[i] = CreateThread(NULL, 0, winApiTask, &iterations, 0, NULL);
}
// Wait for completion
WaitForMultipleObjects(numThreads, threads.data(), TRUE, INFINITE);
// Clean up
for(HANDLE h : threads) {
CloseHandle(h);
}
auto end = std::chrono::high_resolution_clock::now();
return std::chrono::duration<double, std::milli>(end - start).count();
}
// Benchmark std::thread
double benchmarkStdThreads(int numThreads, int iterations) {

std::vector<std::thread> threads;
auto start = std::chrono::high_resolution_clock::now();
// Create threads
for(int i = 0; i < numThreads; i++) {
threads.emplace_back(simpleTask, iterations);
}
// Wait for completion
for(auto& t : threads) {
t.join();
}
auto end = std::chrono::high_resolution_clock::now();
return std::chrono::duration<double, std::milli>(end - start).count();
}
int main() {
const int NUM_THREADS = 8;
const int ITERATIONS = 1000000;
const int TEST_RUNS = 5;
std::cout << "Threading Performance Comparison" << std::endl;
std::cout << "Threads: " << NUM_THREADS << std::endl;
std::cout << "Iterations perthread: " << ITERATIONS << std::endl;
std::cout << "Testruns: " << TEST_RUNS << std::endl;
std::cout << std::string(50, '=') << std::endl;
double winApiTotal = 0, stdThreadTotal = 0;
for(intrun = 0;run < TEST_RUNS;run++) {
std::cout << "Run " << (run + 1) << ":" << std::endl;
// Benchmark Windows APIthreads
double winApiTime = benchmarkWinApiThreads(NUM_THREADS, ITERATIONS);
winApiTotal += winApiTime;
std::cout << " Windows API: " << winApiTime << " ms" << std::endl;
// Benchmark std::thread
double stdThreadTime = benchmarkStdThreads(NUM_THREADS, ITERATIONS);
stdThreadTotal += stdThreadTime;
std::cout << " std::thread: " << stdThreadTime << " ms" << std::endl;
std::cout << std::endl;
}

Compilation Instructions
For Process Examples:

For Thread Examples:
// Calculate averages
double winApiAvg = winApiTotal / TEST_RUNS;
double stdThreadAvg = stdThreadTotal / TEST_RUNS;
std::cout << "Average Results:" << std::endl;
std::cout << "Windows API threads: " << winApiAvg << " ms" << std::endl;
std::cout << "std::thread: " << stdThreadAvg << " ms" << std::endl;
double difference = std::abs(winApiAvg - stdThreadAvg);
double percentDiff = (difference / std::min(winApiAvg, stdThreadAvg)) * 100;
std::cout << "Difference: " << difference << " ms (" << percentDiff << "%)" << std::endl;
if (winApiAvg < stdThreadAvg) {
std::cout << "Windows API threads are faster" << std::endl;
} else {
std::cout << "std::thread is faster" << std::endl;
}
return 0;
}