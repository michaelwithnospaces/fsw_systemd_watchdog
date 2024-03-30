1. Install Dependencies

	sudo pacman -Syu
	sudo pacman -S cmake base-devel systemd

2. Compile the Program

Next, navigate to the directory containing your code and use CMake to build the fsw executable:

# Create a build directory and navigate into it
	mkdir build && cd build

# Run CMake to configure the build environment
	cmake ..

# Compile the program
	make

This process will create an executable named fsw in the build directory.

3. Create a systemd Service File

To run your program as a service and make use of the watchdog functionality, you'll need to create a systemd service file.


	[Unit]
	Description=FSW Service with Watchdog
	After=network.target

	[Service]
	Type=notify
	ExecStart=/path/to/your/build/directory/fsw
	WatchdogSec=10s
	Restart=on-failure
	NotifyAccess=all

	[Install]
	WantedBy=multi-user.target

Replace /path/to/your/build/directory/fsw with the actual path to your compiled fsw executable. The WatchdogSec parameter specifies the watchdog interval (in this case, 10 seconds), which your program must adhere to.

4. Enable and Start the Service

Copy the service file to /etc/systemd/system/ and then use systemctl to enable and start the service:

	sudo cp fsw.service /etc/systemd/system/
	sudo systemctl daemon-reload
	sudo systemctl enable fsw.service
	sudo systemctl start fsw.service

5. Check the Service Status

To verify that the service is running correctly and interacting with the watchdog:

	systemctl status fsw.service
