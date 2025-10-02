#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <random>
#include <chrono>
class BankAccount {
private:
double balance;
std::mutex mtx;
public:
BankAccount(double initialBalance) : balance(initialBalance) {}
void deposit(double amount, int customerId) {
std::lock_guard<std::mutex> lock(mtx);
balance += amount;
std::cout << "Customer " << customerId << " deposited $" << amount
<< " - New balance: $" << balance << std::endl;
}
bool withdraw(double amount, int customerId) {
std::lock_guard<std::mutex> lock(mtx);
if (balance >= amount) {
balance -= amount;
std::cout << "Customer " << customerId << " withdrew $" << amount
<< " - New balance: $" << balance << std::endl;
return true;
} else {
std::cout << "Customer " << customerId << " failed to withdraw $" << amount
<< " - Insufficient funds (Balance: $" << balance << ")" << std::endl;
return false;
}
}
double getBalance() {
std::lock_guard<std::mutex> lock(mtx);
return balance;
}
};
void customerThread(BankAccount& account, int customerId) {
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> amountDist(10.0, 200.0);

std::uniform_int_distribution<> operationDist(0, 1); // 0 = withdraw, 1 = deposit
for(int i = 0; i < 5; i++) {
double amount = amountDist(gen);
int operation = operationDist(gen);
if (operation == 0) {
account.withdraw(amount, customerId);
} else {
account.deposit(amount, customerId);
}
// Random delay between transactions
std::this_thread::sleep_for(std::chrono::milliseconds(100 + (customerId * 50)));
}
}
int main() {
const double INITIAL_BALANCE = 1000.0;
const int NUM_CUSTOMERS = 5;
BankAccount account(INITIAL_BALANCE);
std::vector<std::thread> customers;
std::cout << "Banking System Simulation Started" << std::endl;
std::cout << "Initial Balance: $" << INITIAL_BALANCE << std::endl;
std::cout << "Number of Customers: " << NUM_CUSTOMERS << std::endl;
std::cout << std::string(50, '=') << std::endl;
// Create customerthreads
for(int i = 0; i < NUM_CUSTOMERS; i++) {
customers.emplace_back(customerThread, std::ref(account), i + 1);
}
// Wait for all customers to complete transactions
for(auto& customer: customers) {
customer.join();
}
std::cout << std::string(50, '=') << std::endl;
std::cout << "All transactions completed!" << std::endl;
std::cout << "Final Balance: $" << account.getBalance() << std::endl;
return 0;
}