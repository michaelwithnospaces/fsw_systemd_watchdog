#include "watchdog.h"

int i = 0;
uint64_t watchdogIntervalInUs;

int WatchDogInit() {
    setbuf(stdout, NULL);

    const bool watchdog = sd_watchdog_enabled(0, &watchdogIntervalInUs) > 0; // Service WatchdogSec must be set for this to return > 0
    if (watchdog) {
        printf("Watchdog is enabled with %lu us\n", watchdogIntervalInUs);
    } else {
        printf("Not running in a watchdog env\n");
        return 1;
    }

    // To be able to use sd_notify at all have to set service NotifyAccess (e.g. to main)
    sd_notify(0, "READY=1"); // If service Type=notify the service is only considered ready once we send this (this is independent of watchdog capability)
    printf("Notified as ready. Now sleeping with watchdog\n");
    
    return 0;
}

inline void WatchDogLoop() {
        usleep(watchdogIntervalInUs / 2); // Recommended reporting interval is half the watchdog interval
        sd_notify(0, "WATCHDOG=1");
        sd_notifyf(0, "STATUS=Watchdog notify count %d", i); // Visible in systemctl status
        ++i;
}