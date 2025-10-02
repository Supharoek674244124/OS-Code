#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
class Counter{
private:
int count;
std::mutex mtx;
public:
Counter() : count(0) {}
void increment() {
std::lock_guard<std::mutex> lock(mtx);
count++;
std::cout << "Count: " << count << " (Thread: " << std::this_thread::get_id() << ")" << std::endl;
}
int getCount() {
std::lock_guard<std::mutex> lock(mtx);
return count;
}
};
void incrementCounter(Counter& counter, int iterations) {
for(int i = 0; i < iterations; i++) {
counter.increment();
std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
}
int main() {
Counter counter;
const int NUM_THREADS = 3;
const int ITERATIONS_PER_THREAD = 5;
std::vector<std::thread> threads;
// Create threads
for(int i = 0; i < NUM_THREADS; i++) {
threads.emplace_back(incrementCounter, std::ref(counter), ITERATIONS_PER_THREAD);
}
// Wait for completion
for(auto& t : threads) {
    t.join();
}
std::cout << "Final count: " << counter.getCount() << std::endl;
return 0;
}