#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "Mock Trading Engine Initialized." << std::endl;
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    return 0;
}