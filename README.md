#arduino-energy-monitor

##Working features
- Assigns address via DHCP and prints it on the LCD.

##TODO
- Read temperature and humidity sensor data.
- Read energy sensor data.
- Hold a running average of all sensors (especially energy) in memory to avoid major spikes in readings.
- Cycle through sensor data on the bottom row of the LCD
- Respond to HTTP GET requests with live sensor data in JSON format.
