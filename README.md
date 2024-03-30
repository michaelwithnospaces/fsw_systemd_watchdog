# Installation Instructions for Arch Linux

This guide walks you through setting up the `fsw` (File System Watcher) on an Arch Linux distribution. Follow these steps to install dependencies, compile the program, and run it as a systemd service.

## Prerequisites

Before you begin, ensure you have a working Arch Linux setup and that you have sudo privileges.

## 1. Install Dependencies

Update your system and install the necessary packages:

```bash
sudo pacman -Syu
sudo pacman -S cmake base-devel systemd
```

## 2. Configure `fsw.cpp`

You need to specify the path to the log file in `fsw.cpp`:

- Open `fsw.cpp` in your favorite text editor.
- Find the line containing `logFile.open("//path/to/logfile/fsw.log", std::ios::app);`.
- Replace `"/path/to/logfile/"` with the path where you wish to store your log file, ensuring the specified location has the appropriate write permissions.

## 3. Compile the Program

Follow these steps to compile `fsw`:

1. **Create and Enter Build Directory**
    ```bash
    mkdir build && cd build
    ```
   
2. **Configure Build with CMake**
    ```bash
    cmake ..
    ```

3. **Compile**
    ```bash
    make
    ```
   
   This will create the `fsw` executable within the `build` directory.

## 4. Systemd Service

To manage `fsw` as a systemd service with watchdog functionality:

1. **Open `fsw.service` File**

    Replace `/path/to/your/build/directory/fsw` with the actual path to the `fsw` executable. The `WatchdogSec=10s` specifies the service should be checked every 10 seconds.

    ```
    ExecStart=/path/to/your/build/directory/fsw
    ```

2. **Enable and Start Service**
   
   Copy your service file to the systemd directory, reload systemd to recognize your new service, enable it to start at boot, and then start the service:

    ```bash
    sudo cp fsw.service /etc/systemd/system/
    sudo systemctl daemon-reload
    sudo systemctl enable fsw.service
    sudo systemctl start fsw.service
    ```

## 5. Verify Service Status

Check the status of your `fsw` service to ensure it's running as expected:

```bash
systemctl status fsw.service
```






# For Debian Linux Distro

## 1. Install Dependencies

Debian uses the `apt` package manager instead of `pacman`. To install the necessary dependencies:

```bash
sudo apt update && sudo apt upgrade
sudo apt install cmake build-essential systemd
```

## 2. Edit `fsw.cpp`

Change the log file path in `fsw.cpp` to point to your desired location. This involves modifying the `fopen` function call:

Find the line:
```cpp
logFile.open("//path/to/logfile/fsw.log", std::ios::app);
```

Replace `"/path/to/logfile/"` with the actual path where you want the log file, ensuring the path has proper write permissions.

## 3. Compile the Program

To compile your program, follow these steps:

- Create a build directory and navigate into it:
```bash
mkdir build && cd build
```

- Run CMake to configure the build environment:
```bash
cmake ..
```

- Compile the program:
```bash
make
```

This process will create an executable named `fsw` in the `build` directory.

## 4. Open fsw.service File

Replace `/path/to/your/build/directory/fsw` with the actual path to the `fsw` executable. The `WatchdogSec=10s` specifies the service should be checked every 10 seconds.

    ```
    ExecStart=/path/to/your/build/directory/fsw
    ```

## 5. Enable and Start the Service

To enable and start your new service:

```bash
sudo cp fsw.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable fsw.service
sudo systemctl start fsw.service
```

## 6. Check the Service Status

To verify that the service is running correctly and is interacting with the watchdog:

```bash
systemctl status fsw.service
```

This adjusted guide takes into account Debian's system architecture, ensuring a smooth setup process for Debian users.
