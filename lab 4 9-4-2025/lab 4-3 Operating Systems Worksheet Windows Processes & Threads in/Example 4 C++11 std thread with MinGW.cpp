#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
std::mutex printMutex; // Forthread-safe printing
void workerThread(int threadId) {
    for(int i = 0; i < 3; i++) {
// Lock forthread-safe output
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << "Thread " << threadId << " working... iteration " << i << std::endl;
// Unlock automatically when lock_guard goes out of scope
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
}
int main() {
    const int NUM_THREADS = 4;
    std::vector<std::thread> threads;
// Create and start threads
        for(int i = 0; i < NUM_THREADS; i++) {
    threads.emplace_back(workerThread, i + 1);
}
// Wait for allthreads to complete
for(auto& t : threads) {
    t.join();
}
    std::cout << "All threads finished!" << std::endl;
return 0;
}