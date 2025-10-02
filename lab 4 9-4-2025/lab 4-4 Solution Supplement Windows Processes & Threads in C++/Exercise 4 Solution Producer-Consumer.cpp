#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <random>
class ProducerConsumer
{
private:
    std::queue<int> buffer;
    std::mutex mtx;
    std::condition_variable cv_producer, cv_consumer;
    const size_t MAX_BUFFER_SIZE = 5;
    bool finished = false;

public:
    void producer(int id, int items)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);
        for (int i = 0; i < items; i++)
        {
            std::unique_lock<std::mutex> lock(mtx);
            // Wait if bufferis full
            cv_producer.wait(lock, [this]
                             { return buffer.size() < MAX_BUFFER_SIZE; });
            int item = dis(gen);
            buffer.push(item);
            std::cout << "Producer " << id << " produced: " << item
                      << " (Buffer size: " << buffer.size() << ")" << std::endl;
            cv_consumer.notify_one();
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    void consumer(int id, int items)
    {
        for (int i = 0; i < items; i++)
        {
            std::unique_lock<std::mutex> lock(mtx);
            // Wait if bufferis empty
            cv_consumer.wait(lock, [this]
                             { return !buffer.empty(); });
            int item = buffer.front();
            buffer.pop();
            std::cout << "Consumer " << id << " consumed: " << item
                      << " (Buffer size: " << buffer.size() << ")" << std::endl;
            cv_producer.notify_one();
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
    }
};
int main()
{
    ProducerConsumer pc;
    const int ITEMS_PER_THREAD = 5;
    // Create threads
    std::thread producer1(&ProducerConsumer::producer, &pc, 1, ITEMS_PER_THREAD);
    std::thread producer2(&ProducerConsumer::producer, &pc, 2, ITEMS_PER_THREAD);
    std::thread consumer1(&ProducerConsumer::consumer, &pc, 1, ITEMS_PER_THREAD);
    std::thread consumer2(&ProducerConsumer::consumer, &pc, 2, ITEMS_PER_THREAD);
    // Wait for completion
    producer1.join();
    producer2.join();
    consumer1.join();
    consumer2.join();
    std::cout << "Producer-Consumer simulation completed!" << std::endl;
    return 0;
}