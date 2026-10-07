#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <chrono>

// Background worker function that runs independently
void writeToLogFile(std::string message) {
    // Simulate slow file I/O or network write
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::ofstream logFile("app_events.log", std::ios::app);
    if (logFile.is_open()) {
        logFile << "[LOG] " << message << std::endl;
    }
}

// Function called when a user performs an action in the application
void logEventAsync(std::string eventDescription) {
    // 1. Pass data by value so the thread owns its own copy of the string
    // 2. Spawn the thread
    std::thread logThread(writeToLogFile, eventDescription);

    // 3. Detach it! 
    // This hands off management to the OS runtime and allows logEventAsync()
    // to return INSTANTLY back to the caller without waiting for disk write.
    logThread.detach();
}

int main() {
    std::cout << "Application started. Processing user actions...\n";

    // User performs actions; we fire-and-forget background log operations
    logEventAsync("User clicked 'Login' button");
    logEventAsync("User navigated to Profile page");
    logEventAsync("User updated settings");

    std::cout << "UI/Main Thread is completely responsive and continuing execution...\n";

    // Keep the main process alive (simulating a long-running app loop like a server or UI app)
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Application exiting gracefully.\n";
    return 0;
}

/*
Your output is garbled and interleaved because std::cout is a shared global resource, 
and your threads are writing to it simultaneously without synchronization, 
creating a data race / resource race on the output stream.
When multiple detached threads execute concurrently, the operating system schedules their execution across CPU cores or time slices. 
Without a lock, thread character streams get mixed together character-by-character or buffer-flush by buffer-flush:

Thread 1: [LOG] User navigated to Profile page\n
Thread 2: [LOG] User clicked 'Login' button\n

Actual Interleaved Output:
[LOG] User navigated to Profi[LOG] User clicked 'Login' button

The Fix: Protect std::cout with a std::mutex
To fix this, introduce a std::mutex (mutual exclusion lock) and lock it using std::lock_guard before writing to std::cout. 
This ensures only one thread can print at any given moment.

*/