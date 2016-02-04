# Arduino Energy Monitor
Early stages of development. Check back later for more info :)

## Working features
- Assigns address via DHCP and prints it on the top row of the LCD.
- Reads temperature and humidity sensor data.
- Reads energy sensor data.
- Cycles through sensor data on the bottom row of the LCD
- Respond to HTTP GET requests with current sensor data in JSON format.

## TODO
- Hold a running average of the energy sensor in memory to avoid major spikes in readings.

## References
### Software
- [LiquidCrystal_I2C](https://github.com/marcoschwartz/LiquidCrystal_I2C) library by [marcoschwartz](https://github.com/marcoschwartz) used for interfacing with the LCD display.
- [EmonLib](https://github.com/openenergymonitor/EmonLib) library by [openenergymonitor](https://github.com/openenergymonitor) used for interfacing with the current sensor.
- [DHT-sensor-library](https://github.com/adafruit/DHT-sensor-library) by [adafruit](https://github.com/adafruit) used for interfacing with the temperature and humidity sensor.

### Hardware
- Current sensing circuit based off design from [OpenEnergyMonitor](http://openenergymonitor.org/emon/buildingblocks/how-to-build-an-arduino-energy-monitor).
