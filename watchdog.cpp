/**
 * @file watchdog.h
 * @brief Interface for systemd watchdog integration for services.
 *
 * Provides an interface to integrate systemd watchdog functionality within a service. This includes
 * initializing the watchdog at service startup and continuously notifying the watchdog to prevent
 * the service from being restarted. It ensures that services conform to systemd's expectations for
 * watchdog-based monitoring, improving reliability and manageability.
 * 
 * @author Michael Nguyen
 * @date 3/30/2024
 */


#include <systemd/sd-daemon.h>
#include <stdio.h>
#include <unistd.h>

int WatchDogInit();
void WatchDogLoop();
