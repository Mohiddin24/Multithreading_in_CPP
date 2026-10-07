#include <iostream>
#include <thread>

void hello() {
    std::cout << "Hello from thread!" << std::endl;
}
int main() {
    std::thread t(hello);
    // If you comment out t.join();, your program will crash at runtime with a std::terminate exception.
    // t.join();
    std::cout << "Hello, World!" << std::endl;
}