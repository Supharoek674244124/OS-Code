#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include <mutex>
#include <map>
#include <string>
#include <chrono>
class FileProcessor{
private:
std::mutex resultMutex;
std::map<std::string, int> totalWordCount;
public:
void processFile(const std::string& filename) {
std::ifstream file(filename);
if (!file.is_open()) {
std::lock_guard<std::mutex> lock(resultMutex);
std::cerr << "Error: Cannot open file " << filename << std::endl;
return;
}
std::map<std::string, int> localWordCount;
std::string word;
int totalWords = 0;
auto start = std::chrono::high_resolution_clock::now();
// Count words in file
while (file >> word) {
// Simple word cleaning (remove punctuation)
word.erase(std::remove_if(word.begin(), word.end(),
[](char c) {return !std::isalnum(c); }), word.end());
if (!word.empty()) {
std::transform(word.begin(), word.end(), word.begin(), ::tolower);
localWordCount[word]++;
totalWords++;
}
}
file.close();
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

// Update globalresults (thread-safe)
{
std::lock_guard<std::mutex> lock(resultMutex);
for(const auto& pair: localWordCount) {
totalWordCount[pair.first] += pair.second;
}
std::cout << "File: " << filename
<< " - Words: " << totalWords
<< " - Time: " << duration.count() << "ms"
<< " - Thread: " << std::this_thread::get_id() << std::endl;
}
}
void printTopWords(int count = 10) {
std::vector<std::pair<std::string, int>> sortedWords(
totalWordCount.begin(), totalWordCount.end()
);
std::sort(sortedWords.begin(), sortedWords.end(),
[](const auto& a, const auto& b) {return a.second > b.second; });
std::cout << "\nTop " << count << " most frequent words:" << std::endl;
std::cout << std::string(40, '-') << std::endl;
for(int i = 0; i < std::min(count, (int)sortedWords.size()); i++) {
std::cout << std::left << std::setw(20) << sortedWords[i].first
<< ": " << sortedWords[i].second << std::endl;
}
}
int getTotalWordCount() const {
int total = 0;
for(const auto& pair: totalWordCount) {
total += pair.second;
}
return total;
}
};
// Function to create sample files fortesting
void createSampleFiles() {
std::vector<std::string> sampleTexts = {
"The quick brown fox jumps overthe lazy dog. The dog was very lazy indeed.",
"Hello world! This is a sample text file. Hello again world!",
"Programming is fun. C++ programming is especially fun. Fun times ahead!",

"Threads are useful for concurrent programming. Concurrent programming requires careful synchronization."
};
for(size_t i = 0; i < sampleTexts.size(); i++) {
    std::ofstream file("sample" + std::to_string(i + 1) + ".txt");
    file << sampleTexts[i];
    file.close();
}
    std::cout << "Created 4 sample files fortesting." << std::endl;
}
int main() {
// Create sample files
    createSampleFiles();
    FileProcessor processor;
    std::vector<std::thread> threads;
    std::vector<std::string> filenames = {
"sample1.txt", "sample2.txt", "sample3.txt", "sample4.txt"
};
auto start = std::chrono::high_resolution_clock::now();
// Process files concurrently
    std::cout << "Processing files with multiple threads..." << std::endl;
    for(const auto& filename : filenames) {
    threads.emplace_back(&FileProcessor::processFile, &processor, filename);
}
// Wait for allthreads to complete
for(auto& t : threads) {
    t.join();
}
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
// Display results
    processor.printTopWords();
    std::cout << "\nTotal words processed: " << processor.getTotalWordCount() << std::endl;
    std::cout << "Total processing time: " << duration.count() << "ms" << std::endl;
return 0;
}