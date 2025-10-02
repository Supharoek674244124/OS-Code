#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;

public:
    ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this, i] {
                std::cout << "Workerthread " << i << " started" << std::endl;
                for (;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->condition.wait(lock, [this] {
                            return this->stop || !this->tasks.empty();
                        });
                        if (this->stop && this->tasks.empty()) {
                            std::cout << "Workerthread " << i << " stopping" << std::endl;
                            return;
                        }
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    template <class F>
    auto enqueue(F&& f) -> std::future<typename std::invoke_result<F>::type> {
        using return_type = typename std::invoke_result<F>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(std::forward<F>(f));
        std::future<return_type> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop) {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers) {
            worker.join();
        }
    }
};

// Example task functions
int computeFactorial(int n) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int result = 1;   // ✅ fixed
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    std::cout << "Factorial of " << n << " = " << result << std::endl;
    return result;
}

void printMessage(const std::string &msg) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "Message: " << msg 
              << " [Thread: " << std::this_thread::get_id() << "]" << std::endl;
}

int main() {
    ThreadPool pool(4);
    std::vector<std::future<int>> factorialResults;
    std::vector<std::future<void>> messageResults;

    std::cout << "Starting thread pool with 4 workers" << std::endl;

    // Enqueue factorial calculations
    for (int i = 1; i <= 6; i++) {
        factorialResults.emplace_back(
            pool.enqueue([i] { return computeFactorial(i); })
        );
    }

    // Enqueue message printing tasks
    for (int i = 1; i <= 5; i++) {
        messageResults.emplace_back(
            pool.enqueue([i] { printMessage("Hello from task " + std::to_string(i)); })
        );
    }

    // Wait for factorial results
    for (auto &result : factorialResults) {
        result.wait();
    }

    // Wait for message tasks
    for (auto &result : messageResults) {
        result.wait();
    }

    std::cout << "All tasks completed!" << std::endl;
    return 0;
}
