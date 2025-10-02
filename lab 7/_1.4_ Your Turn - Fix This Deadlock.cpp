#include <iostream>
#include <thread>
#include <mutex>
#include <chrono> 

std::mutex resourceA, resourceB, resourceC;

void process1() {
    std::lock(resourceA, resourceB); 
    std::lock_guard<std::mutex> lockA(resourceA, std::adopt_lock);
    std::lock_guard<std::mutex> lockB(resourceB, std::adopt_lock);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "process1 working\n";
}

void process2() {
    std::lock(resourceB, resourceC);
    std::lock_guard<std::mutex> lockB(resourceB, std::adopt_lock);
    std::lock_guard<std::mutex> lockC(resourceC, std::adopt_lock);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "process2 working\n";
}

void process3() {
    std::lock(resourceA, resourceC);
    std::lock_guard<std::mutex> lockA(resourceA, std::adopt_lock);
    std::lock_guard<std::mutex> lockC(resourceC, std::adopt_lock);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "process3 working\n";
}

int main() {
    std::thread t1(process1);
    std::thread t2(process2);
    std::thread t3(process3);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "All processes finished.\n";
    return 0;
}
