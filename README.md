# GPS-NodeMcu

## Overview

This Arduino sketch enables an ESP8266 microcontroller (tested on NodeMCU 1.0 board) to function as a GPS data logger using the NEO-7M GPS module and TinyGPS++ library. The device connects to a WiFi network, retrieves GPS coordinates (latitude, longitude), date, and time from the GPS module, converts UTC time to Indian Standard Time (IST), and serves this information via a basic HTML page over WiFi.

> [!WARNING]
> This code is for hobbyists for learning purposes. Not for production use!!

## Features

- **GPS Data Retrieval:** Parses latitude, longitude, date, and time from NEO-7M GPS module.
- **WiFi Connectivity:** Connects to a specified WiFi network for serving GPS data over HTTP.
- **Time Conversion:** Converts UTC time to Indian Standard Time (IST) for local time display.
- **HTML Response:** Generates a simple HTML page displaying GPS location details.

## Hardware Requirements

- ESP8266 (tested on NodeMCU 1.0 board, ESP12E-Module)
- NEO-7M GPS module (connected via software serial, RX on D5, TX on D6)

## Libraries Used

- TinyGPS++: For parsing GPS data.
- SoftwareSerial: For communication with NEO-7M GPS module.
- ESP8266WiFi: For WiFi connectivity.

## Setup

1. **Hardware Connections:**
   - Connect NEO-7M GPS module to ESP8266:
     - GPS Module RX -> ESP8266 D5
     - GPS Module TX -> ESP8266 D6

2. **Software Setup:**
   - Install necessary libraries (TinyGPS++, SoftwareSerial, ESP8266WiFi).
   - Upload the provided Arduino sketch (`GPS3.0.ino`) to your ESP8266 board.

3. **Configure WiFi:**
   - Modify `YOUR_WIFI_SSID` and `YOUR_WIFI_PASSWORD` variables in the sketch to match your WiFi network credentials.

4. **Upload and Run:**
   - Upload the sketch to your ESP8266 board using the Arduino IDE.
   - Open the Serial Monitor to view debug messages and obtain the assigned IP address.
   - Access the GPS data served over WiFi by navigating to the ESP8266 IP address in a web browser.

## License

This project is licensed under the [MIT License](LICENSE).

## Contributing

Contributions are welcome! For major changes, please open an issue first to discuss what you would like to change.

## Acknowledgments

- [TinyGPS++ library](https://github.com/mikalhart/TinyGPSPlus) for GPS data parsing.
- [ESP8266WiFi library](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi) for WiFi connectivity.
- [Arduino IDE](https://www.arduino.cc/en/Main/Software) for development environment.

---

Feel free to customize the `README.md` further with additional details, troubleshooting tips, or any specific instructions relevant to your project setup and usage.
