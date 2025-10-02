#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
std::mutex outputMutex;
// Function to calculate factorial
long long factorial(int n) {
long long result = 1;
for(int i = 1; i <= n; i++) {
result *= i;
}
return result;
}
// Workerthread function
void workerThread(int threadId) {
std::lock_guard<std::mutex> lock(outputMutex);
std::cout << "Workerthread " << threadId << " started" << std::endl;
for(int i = 1; i <= 10; i++) {
long long fact = factorial(i);
std::cout << "Thread " << threadId << ": " << i << "! = " << fact << std::endl;
// Small delay to make output visible
std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
std::cout << "Workerthread " << threadId << " completed" << std::endl;
}
int main() {
const int NUM_THREADS = 4;
std::vector<std::thread> workers;
std::cout << "Creating " << NUM_THREADS << " workerthreads..." << std::endl;
// Create workerthreads
for(int i = 0; i < NUM_THREADS; i++) {
workers.emplace_back(workerThread, i + 1);
}
std::cout << "Main thread waiting for all workers to complete..." << std::endl;

Question 6 Solution:
Race Conditions: A race condition occurs when multiple threads access shared data concurrently, and the
final result depends on the timing of their execution.
Example:
// Wait for allthreads to complete
for(auto& worker: workers) {
worker.join();
}
std::cout << "All workerthreads completed!" << std::endl;
return 0;
}