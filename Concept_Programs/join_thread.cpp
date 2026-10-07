#include <iostream>
#include <thread>

void task() {
    std::cout << "Thread working...\n";
}

int main() {
    std::thread t(task);

    t.join(); // Blocks main() until task() completes
    std::cout << "Thread finished. Main continuing.\n";
    return 0; 
}