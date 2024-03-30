#include <systemd/sd-daemon.h>
#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <ctime>
#include <cstring>

// void writeDateToLog() {
//     FILE* logFile;
//     logFile = fopen("/home/michael/test/fsw.log", "w");

//     // Get current time and format it
//     auto now = std::chrono::system_clock::now();
//     auto now_c = std::chrono::system_clock::to_time_t(now);
//     // logFile << std::ctime(&now_c) << " - FSW task executed" << std::endl;

//     fprintf(logFile, "%s - FSW task executed\n", std::ctime(&now_c));
//     fclose(logFile);
// }

void writeDateToLog() {
    FILE* logFile;
    logFile = fopen("/home/michael/test/fsw.log", "a"); // Open the file in append mode

    // Get current time and format it
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    char* timeStr = std::ctime(&now_c);

    // Removed the newline character from the end of the time string
    size_t len = strlen(timeStr);
    if (len > 0 && timeStr[len - 1] == '\n') {
        timeStr[len - 1] = '\0';
    }

    fprintf(logFile, "%s - FSW task executed\n", timeStr);
    fclose(logFile);
}

// void writeCurrentDateTimeToLogFile() {
//     // Open the log file in append mode
//     std::ofstream logFile("/var/tmp/fsw.log", std::ios_base::app);
//     if (!logFile.is_open()) {
//         perror("Failed to open log file");
//         return;
//     }
//     else {
//         printf("Log file opened successfully.\n");
//     }

//     // Get current time and format it
//     auto now = std::chrono::system_clock::now();
//     auto now_c = std::chrono::system_clock::to_time_t(now);
//     logFile << std::ctime(&now_c) << " - FSW task executed" << std::endl;

//     if (!logFile.good()) {
//         std::perror("Failed to write to log file");
//     }
//     else {
//         printf("Log entry written successfully.\n");
//     }
// }

int main() {
    setbuf(stdout, NULL);

    uint64_t watchdogIntervalInUs;
    const bool watchdog = sd_watchdog_enabled(0, &watchdogIntervalInUs) > 0;
    if (watchdog) {
        printf("Watchdog is enabled with %lu us\n", watchdogIntervalInUs);
    } else {
        printf("Not running in a watchdog env\n");
        return 1;
    }

    sd_notify(0, "READY=1");
    printf("Notified as ready. Now sleeping with watchdog\n");

    int i = 0;
    while(true) {
        auto taskStart = std::chrono::high_resolution_clock::now();

        // Perform the task of writing the current date and time to a log file
        writeDateToLog();

        // Calculate the elapsed time for the task
        auto taskEnd = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(taskEnd - taskStart).count();

        // Calculate the remaining time to sleep to maintain the 5-second cycle as much as possible
        uint64_t sleepTime = 5000000 - elapsed; // 5 seconds in microseconds - elapsed time of the task
        if (sleepTime > watchdogIntervalInUs / 2) {
            sleepTime = watchdogIntervalInUs / 2; // Ensure we don't exceed half the watchdog interval
        }

        usleep(watchdogIntervalInUs / 2); // Recommended reporting interval is half the watchdog interval
        
        // notifies systemd watchdog
        sd_notify(0, "WATCHDOG=1");
        sd_notifyf(0, "STATUS=Watchdog notify count %d", i); // Visible in systemctl status
        ++i;
    }
    
    return 0;
}