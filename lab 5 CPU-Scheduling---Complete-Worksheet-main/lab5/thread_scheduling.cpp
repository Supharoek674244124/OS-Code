// File: thread_scheduling.cpp
// Compile: g++ -o thread_scheduling thread_scheduling.cpp -std=c++17 -pthread

#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>

class ThreadInfo {
public:
    int thread_id;
    int priority;
    int burst_time;
    std::chrono::steady_clock::time_point arrival_time;
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point completion_time;
    
    ThreadInfo(int id, int prio, int burst) 
        : thread_id(id), priority(prio), burst_time(burst) {
        arrival_time = std::chrono::steady_clock::now();
    }
};

class ThreadScheduler {
private:
    std::queue<ThreadInfo> ready_queue;
    std::mutex queue_mutex;
    std::condition_variable cv;
    std::atomic<bool> running{true};
    std::atomic<int> active_threads{0};
    
public:
    void addThread(ThreadInfo thread_info) {
        std::lock_guard<std::mutex> lock(queue_mutex);
        ready_queue.push(thread_info);
        active_threads++;
        cv.notify_one();
    }
    
    void scheduler() {
        while (running || active_threads > 0) {
            std::unique_lock<std::mutex> lock(queue_mutex);
            cv.wait(lock, [this] { return !ready_queue.empty() || !running; });
            
            if (!ready_queue.empty()) {
                ThreadInfo current_thread = ready_queue.front();
                ready_queue.pop();
                lock.unlock();
                
                // Simulate thread execution
                current_thread.start_time = std::chrono::steady_clock::now();
                std::cout << "Executing Thread " << current_thread.thread_id 
                          << " (Priority: " << current_thread.priority << ")\n";
                
                std::this_thread::sleep_for(std::chrono::milliseconds(current_thread.burst_time * 100));
                
                current_thread.completion_time = std::chrono::steady_clock::now();
                
                auto turnaround_time = std::chrono::duration_cast<std::chrono::milliseconds>
                    (current_thread.completion_time - current_thread.arrival_time);
                
                std::cout << "Thread " << current_thread.thread_id 
                          << " completed. Turnaround time: " << turnaround_time.count() << "ms\n";
                
                active_threads--;
            }
        }
    }
    
    void stop() {
        running = false;
        cv.notify_all();
    }
};

// Pthread-style thread attributes simulation
class ThreadAttributes {
public:
    enum SchedulingPolicy { SCHED_FIFO, SCHED_RR, SCHED_OTHER };
    enum ContentionScope { PTHREAD_SCOPE_PROCESS, PTHREAD_SCOPE_SYSTEM };
    
    SchedulingPolicy policy = SCHED_OTHER;
    ContentionScope scope = PTHREAD_SCOPE_SYSTEM;
    int priority = 0;
    
    void setSchedulingPolicy(SchedulingPolicy pol) { policy = pol; }
    void setContentionScope(ContentionScope sc) { scope = sc; }
    void setPriority(int prio) { priority = prio; }
    
    void displayAttributes() {
        std::cout << "Thread Attributes:\n";
        std::cout << "  Policy: " << (policy == SCHED_FIFO ? "FIFO" : 
                                    policy == SCHED_RR ? "Round Robin" : "Other") << "\n";
        std::cout << "  Scope: " << (scope == PTHREAD_SCOPE_PROCESS ? "Process" : "System") << "\n";
        std::cout << "  Priority: " << priority << "\n";
    }
};

// Demo worker thread function
void workerThread(int id, int work_time) {
    std::cout << "Worker Thread " << id << " starting work for " << work_time << "ms\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(work_time));
    std::cout << "Worker Thread " << id << " completed work\n";
}

int main() {
    std::cout << "=== THREAD SCHEDULING DEMONSTRATION ===\n\n";
    
    // Demonstrate thread attributes
    ThreadAttributes attr;
    attr.setSchedulingPolicy(ThreadAttributes::SCHED_RR);
    attr.setContentionScope(ThreadAttributes::PTHREAD_SCOPE_SYSTEM);
    attr.setPriority(5);
    attr.displayAttributes();
    
    std::cout << "\n=== THREAD SCHEDULER SIMULATION ===\n";
    
    ThreadScheduler scheduler;
    
    // Start scheduler in separate thread
    std::thread scheduler_thread(&ThreadScheduler::scheduler, &scheduler);
    
    // Create and schedule threads
    scheduler.addThread(ThreadInfo(1, 3, 5));
    scheduler.addThread(ThreadInfo(2, 1, 3));
    scheduler.addThread(ThreadInfo(3, 2, 4));
    scheduler.addThread(ThreadInfo(4, 1, 2));
    
    // Wait for all threads to complete
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    scheduler.stop();
    scheduler_thread.join();
    
    std::cout << "\n=== PTHREAD STYLE THREADS ===\n";
    
    // Create multiple worker threads
    std::vector<std::thread> workers;
    
    for (int i = 1; i <= 4; i++) {
        workers.emplace_back(workerThread, i, i * 200);
    }
    
    // Wait for all workers to complete
    for (auto& worker : workers) {
        worker.join();
    }
    
    std::cout << "\nAll threads completed!\n";
    return 0;
}