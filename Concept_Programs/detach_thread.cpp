#include <iostream>
#include <thread>

void task() {
    std::cout << "Thread working...\n";
}

int main() {
    std::thread t(task);

    t.detach(); // Detaches immediately, main continues without waiting
    std::cout << "Main continuing.\n";
    return 0; // WARNING: main might exit and kill the process before task() prints!
}

/*
 * Important Safety Note on detach(): If main() exits while a detached thread is still running, 
 * the entire process terminates and forcibly stops the detached thread. 
 * Be extremely careful when passing references or pointers to local variables into a detached thread, 
 * as the local variables may go out of scope while the thread is still trying to read or write to them.
 */