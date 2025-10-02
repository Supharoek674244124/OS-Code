#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <sstream>
struct ClientRequest {
    int clientId;
        std::string requestType;
        std::string resource;
        std::chrono::system_clock::time_point timestamp;
};
struct ServerResponse {
    int clientId;
    int statusCode;
        std::string content;
        std::chrono::milliseconds processingTime;
};
class WebServerSimulation {
private:
    std::queue<ClientRequest> requestQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::mutex outputMutex;
bool serverRunning;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> processingTimeDist;
    std::uniform_int_distribution<> statusCodeDist;
public:
WebServerSimulation() : gen(rd()), processingTimeDist(100, 1000),
statusCodeDist(1, 100), serverRunning(true) {}
// Client thread function - generates requests
    void clientThread(int clientId, int numRequests) {
        std::vector<std::string> requestTypes = {"GET", "POST", "PUT", "DELETE"};
        std::vector<std::string> resources = {"/index.html", "/api/users", "/api/data", "/login", "/dashboard"};

std::uniform_int_distribution<> typeDist(0,requestTypes.size() - 1);
std::uniform_int_distribution<> resourceDist(0,resources.size() - 1);
std::uniform_int_distribution<> delayDist(200, 800);
for(int i = 0; i < numRequests; i++) {
ClientRequestrequest;
request.clientId = clientId;
request.requestType = requestTypes[typeDist(gen)];
request.resource = resources[resourceDist(gen)];
request.timestamp = std::chrono::system_clock::now();
// Add request to queue
{
std::lock_guard<std::mutex> lock(queueMutex);
requestQueue.push(request);
std::lock_guard<std::mutex> outLock(outputMutex);
std::cout << "Client " << clientId << " sent " << request.requestType
<< " " << request.resource << std::endl;
}
cv.notify_one();
std::this_thread::sleep_for(std::chrono::milliseconds(delayDist(gen)));
}
}
// Server workerthread function - processes requests
void serverWorkerThread(int workerId) {
while (serverRunning) {
ClientRequestrequest;
bool hasRequest = false;
// Getrequest from queue
{
std::unique_lock<std::mutex> lock(queueMutex);
cv.wait_for(lock, std::chrono::milliseconds(500),
[this] {return !requestQueue.empty() || !serverRunning; });
if (!requestQueue.empty()) {
request = requestQueue.front();
requestQueue.pop();
hasRequest = true;
}
}
if (hasRequest) {
// Process the request

auto processingStart = std::chrono::high_resolution_clock::now();
// Simulate processing time
std::this_thread::sleep_for(
std::chrono::milliseconds(processingTimeDist(gen))
);
auto processingEnd = std::chrono::high_resolution_clock::now();
auto processingTime = std::chrono::duration_cast<std::chrono::milliseconds>(
processingEnd - processingStart
);
// Generate response
ServerResponse response;
response.clientId = request.clientId;
response.processingTime = processingTime;
// Simulate different status codes
int statusChance = statusCodeDist(gen);
if (statusChance <= 80) {
response.statusCode = 200; // 80% success
response.content = "OK - Resource found";
} else if (statusChance <= 90) {
response.statusCode = 404; // 10% not found
response.content = "Not Found";
} else {
response.statusCode = 500; // 10% server error
response.content = "Internal Server Error";
}
// Outputresponse
{
std::lock_guard<std::mutex> lock(outputMutex);
std::cout << "Worker " << workerId << " processed "
<< request.requestType << " " << request.resource
<< " from Client " << request.clientId
<< " -> Status: " << response.statusCode
<< " (Processing time: " << processingTime.count() << "ms)"
<< std::endl;
}
}
}
std::lock_guard<std::mutex> lock(outputMutex);
std::cout << "Server worker " << workerId << " shutting down" << std::endl;
}

void stopServer() {
serverRunning = false;
cv.notify_all();
}
int getQueueSize() {
std::lock_guard<std::mutex> lock(queueMutex);
return requestQueue.size();
}
};
int main() {
WebServerSimulation server;
const int NUM_CLIENTS = 3;
const int NUM_WORKERS = 2;
const int REQUESTS_PER_CLIENT = 5;
std::vector<std::thread> clientThreads;
std::vector<std::thread> workerThreads;
std::cout << "Starting web server simulation..." << std::endl;
std::cout << "Clients: " << NUM_CLIENTS << ", Workers: " << NUM_WORKERS << std::endl;
std::cout << "Requests per client: " << REQUESTS_PER_CLIENT << std::endl;
std::cout << std::string(60, '=') << std::endl;
// Start workerthreads
for(int i = 0; i < NUM_WORKERS; i++) {
workerThreads.emplace_back(&WebServerSimulation::serverWorkerThread, &server, i + 1);
}
// Start client threads
for(int i = 0; i < NUM_CLIENTS; i++) {
clientThreads.emplace_back(&WebServerSimulation::clientThread, &server, i + 1, REQUESTS_PER_CLIENT);
}
// Wait for all clients to finish
for(auto& t : clientThreads) {
t.join();
}
// Give some time forremaining requests to be processed
std::this_thread::sleep_for(std::chrono::seconds(2));
// Stop server
server.stopServer();
// Wait for workers to finish

Lab Assignment Solutions
Section A: Process Management Solutions
Question 1 Solution:
Difference between Process and Program:
A program is a passive entity - executable code stored on disk
A process is an active entity - a program in execution with allocated resources
What happens when you double-click an executable:
1. Operating system creates a new process
2. Allocates memory space for the process
3. Loads the executable code into memory
4. Creates Process Control Block (PCB)
5. Initializes process state and starts execution
Question 2 Solution:
for(auto& t : workerThreads) {
t.join();
}
std::cout << std::string(60, '=') << std::endl;
std::cout << "Web server simulation completed!" << std::endl;
std::cout << "Remaining requests in queue: " << server.getQueueSize() << std::endl;
return 0;
}