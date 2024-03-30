#include <systemd/sd-daemon.h>
#include <stdio.h>
#include <unistd.h>

int WatchDogInit();
void WatchDogLoop();