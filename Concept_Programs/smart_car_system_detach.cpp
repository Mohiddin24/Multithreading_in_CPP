#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <mutex>

std::mutex consoleMutex; // Mutex to synchronize console output across threads

void sendTelemetryData(int currentSpeed, std::string location) {
    // Simulate latency (e.g, sending data over cellular network)
    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    // Protect output stream access with a mutex to avoid garbled output
    {
        std::lock_guard<std::mutex> lock(consoleMutex);
        std::cout << "[TELEMETRY DETACHED] Speed: " << currentSpeed << " km/h, Location: " << location << std::endl;
    }
}

void triggerTelemetry(int speed, std::string loc) {
    // Pass arguments BY VALUE so thread has its own isloated data on its stack
    std::thread telemtryThread(sendTelemetryData, speed, loc);
    telemtryThread.detach(); // Detach the thread to run independently
}

int main() {
    std::cout << "--- Smart Car ECU Initialized ---" << std::endl;

    // Driver accelrates, car speed increases, and location changes
    triggerTelemetry(60, "Highway 101");

    //Main drive thread continues instantly without lagging
    std::cout << "[ENGINE] Engine responding instantly. Steering and brakes active." << std::endl;  
    
    // Driver speeds up further
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << "[ENGINE] Speeding up to 90 km/h..." << std::endl;
    triggerTelemetry(90, "Highway 101 - Exit 12");

    // Keep the main thread alive long enough for detached threads to finish their work
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout<< "\n---Car Parked / Engine Off---" << std::endl;
}