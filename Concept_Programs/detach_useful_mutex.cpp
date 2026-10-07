#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <chrono>
#include <mutex> // 1. Include mutex header

std::mutex logMutex; // 2. Global or static mutex to synchronize console output

void writeToLogFile(std::string message) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::ofstream logFile("app_events.log", std::ios::app);
    if (logFile.is_open()) {
        logFile << "[LOG] " << message << std::endl;
    }

    // 3. Acquire lock before writing to shared std::cout
    {
        std::lock_guard<std::mutex>  (logMutex);
        std::cout << "[LOG] " << message << std::endl;
    } // Lock is automatically released here when going out of scope
}

void logEventAsync(std::string eventDescription) {
    std::thread logThread(writeToLogFile, eventDescription);
    logThread.detach();
}

int main() {
    logEventAsync("User clicked 'Login' button");
    logEventAsync("User navigated to Profile page");

    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}


/* 
Why This Fixes It

1. `std::mutex logMutex`: Acts as a shared gatekeeper across all threads.

2. `std::lock_guard<std::mutex> lock(logMutex)`: When Thread 1 reaches this line, it locks the mutex.

3. If Thread 2 tries to print at the same time, it is paused until Thread 1 finishes printing and unlocks the mutex, 
guaranteeing clean, unbroken lines of output.



The print order changed because **thread scheduling is non-deterministic**.

When you spawn two independent threads, you hand off control of their execution to the operating system's thread scheduler. The OS determines when each thread gets CPU execution time based on hardware availability, system load, and thread prioritization.

---

### Key Reasons for the Swapped Order

1. **Independent Thread Spawning:** Calling `logEventAsync(...)` twice creates two separate OS threads. The order in which threads are created does not guarantee the order in which they acquire CPU time slices or finish execution.


2. **OS Scheduling & Multi-core Execution:** On a multi-core processor, the OS may run Thread 2 on Core A slightly faster than Thread 1 on Core B, or Thread 2 might be scheduled first by the OS thread scheduler.


3. **Mutex Lock Acquisition:** When both threads finish their sleep period and reach the `std::lock_guard<std::mutex>`, whichever thread happens to hit the mutex standard library call a few nanoseconds earlier will acquire the lock first and print its message to `std::cout`.

---

### Is This Normal?

**Yes.** In multithreaded programming, unless you explicitly enforce a strict order using synchronization primitives (like condition variables or promises), the execution order of concurrent threads is inherently unpredictable.

*/

