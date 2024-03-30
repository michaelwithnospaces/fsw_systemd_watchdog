/**
 * @file fsw.cpp
 * @brief This program acts as a systemd watchdog service, regularly reporting its status to systemd and writing timestamps to a log file.
 *
 * It is designed to work with systemd's watchdog feature, ensuring the service is alive by periodically notifying systemd.
 * Additionally, it logs the current date and time to a file, demonstrating a simple but continuous task.
 *
 * @author Michael Nguyen
 * @date 3/30/24
 */

// Include necessary headers for the functionality
#include <systemd/sd-daemon.h> // For systemd watchdog interactions
#include <stdio.h>             // For basic input and output operations (e.g., printf)
#include <unistd.h>            // For usleep (sleeping for microseconds)
#include <chrono>              // For high-resolution clock and time calculations
#include <fstream>             // For file writing operations using ofstream
#include <ctime>               // For converting system time to human-readable format
#include <cstring>             // For string manipulation functions like strlen

/**
 * @brief Function to write the current date and time to a log file
 *
 * @author Michael Nguyen
 * @date 3/30/24
 */
void writeDateToLog() {
    std::ofstream logFile; // Create an ofstream object to handle the file writing
    // Open 'fsw.log' in append mode, creating the file if it doesn't exist
    logFile.open("//path/to/logfile/fsw.log", std::ios::app); 

    // Get the current time as a time_point object
    auto now = std::chrono::system_clock::now();
    // Convert time_point to time_t for converting to readable format
    auto now_c = std::chrono::system_clock::to_time_t(now);
    // Convert time_t to a human-readable string
    char* timeStr = std::ctime(&now_c);

    // Remove the newline character from the end of the time string, if present
    size_t len = strlen(timeStr);
    if (len > 0 && timeStr[len - 1] == '\n') {
        timeStr[len - 1] = '\0';
    }

    // Write the formatted time string to the file followed by a custom message
    logFile << timeStr << " - FSW task executed\n";
    logFile.close(); // Close the file to save changes
}

/**
 * @brief Main loop
 *
 * @author Michael Nguyen
 * @date 3/30/24
 */
int main() {
    setbuf(stdout, NULL); // Disable buffering for stdout to see output immediately

    uint64_t watchdogIntervalInUs; // Variable to store the watchdog interval in microseconds
    // Check if watchdog is enabled and get the interval if it is
    const bool watchdog = sd_watchdog_enabled(0, &watchdogIntervalInUs) > 0;
    if (watchdog) {
        // If watchdog is enabled, print the interval
        printf("Watchdog is enabled with %lu us\n", watchdogIntervalInUs);
    } else {
        // If not running in a systemd watchdog environment, exit with error
        printf("Not running in a watchdog env\n");
        return 1;
    }

    // Notify systemd that the service is ready
    sd_notify(0, "READY=1");
    printf("Notified as ready. Now sleeping with watchdog\n");

    int i = 0; // Initialize a counter for watchdog notifications
    while(true) { // Start an infinite loop
        auto taskStart = std::chrono::high_resolution_clock::now(); // Record task start time

        // Perform the main task of writing to the log file
        writeDateToLog();

        // Calculate the elapsed time for the task
        auto taskEnd = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(taskEnd - taskStart).count();

        // Calculate how long to sleep to keep a 5-second cycle, not exceeding half the watchdog interval
        uint64_t sleepTime = 5000000 - elapsed; // 5 seconds in microseconds - elapsed time
        if (sleepTime > watchdogIntervalInUs / 2) {
            sleepTime = watchdogIntervalInUs / 2; // Adjust sleep time if needed
        }

        usleep(sleepTime); // Sleep for the calculated time
        
        // Notify systemd watchdog
        sd_notify(0, "WATCHDOG=1");
        // Update the status message with the current count
        sd_notifyf(0, "STATUS=Watchdog notify count %d", i); 
        ++i; // Increment the count
    }
    
    return 0; // Although the loop is infinite, this ensures proper form
}
