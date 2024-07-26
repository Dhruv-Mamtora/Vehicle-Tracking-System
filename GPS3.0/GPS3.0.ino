/*****************************************************
 * GPS Data Logger with ESP8266 and TinyGPS++(GPS NEO 7M)
 * 
 * Disclaimer: This code is for hobbyists for learning purposes. Not recommended for production use!!
 * 
 * Hardware Setup:
 * - Tested with GPS module NEO-7M connected to ESP8266 via software serial (RX on D5, TX on D6)
 * - NodeMCU 1.0 board (ESP12E-Module)
 * 
 * Functionality:
 * - Connects to specified WiFi network using credentials
 * - Retrieves GPS data (latitude, longitude, date, time) from NEO-7M GPS module
 * - Converts UTC time to Indian Standard Time (IST)
 * - Serves GPS data via an HTML page over WiFi
 * 
 * Libraries Used:
 * - TinyGPS++: For parsing GPS data
 * - SoftwareSerial: For communication with NEO-7M GPS module
 * - ESP8266WiFi: For WiFi connectivity
 * 
 * Date: 28-March-2024
 *****************************************************/

#include <TinyGPS++.h>         // Library for GPS module
#include <SoftwareSerial.h>    // Library for software serial communication
#include <ESP8266WiFi.h>       // Library for ESP8266 WiFi module

TinyGPSPlus gps;                // Object of TinyGPS++ for GPS data parsing
#define rxGPS D5               // RX pin for GPS module
#define txGPS D6               // TX pin for GPS module

SoftwareSerial ss(rxGPS, txGPS); // Software serial connection to GPS module
const char* ssid = "YOUR_WIFI_SSID";    // Replace with your WiFi SSID
const char* password = "YOUR_WIFI_PASSWORD";  // Replace with your WiFi password

float latitude, longitude;      // Variables to store GPS coordinates
int year, month, date, hour, minute, second; // Variables to store date and time
String date_str, time_str, lat_str, lng_str; // String variables for formatted date, time, latitude, and longitude
int pm;                         // Variable to determine AM/PM

WiFiServer server(80);          // Create a server on port 80

void setup() {
  Serial.begin(115200);         // Initialize serial communication for debugging
  ss.begin(9600);               // Initialize software serial for GPS communication
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);   // Connect to WiFi network

  while (WiFi.status() != WL_CONNECTED) { // Wait until WiFi connection is established
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();               // Start the server
  Serial.println("Server started");
  Serial.println(WiFi.localIP());  // Print the IP address assigned to the ESP8266
}

void loop() {
  while (ss.available() > 0) {  // Check if data is available from GPS module
    if (gps.encode(ss.read())) {  // Read GPS data
      if (gps.location.isValid()) {  // Check if GPS location data is valid
        latitude = gps.location.lat(); // Get latitude
        longitude = gps.location.lng(); // Get longitude
        lat_str = String(latitude, 6); // Convert latitude to string with 6 decimal places
        lng_str = String(longitude, 6); // Convert longitude to string with 6 decimal places
      }

      if (gps.date.isValid()) {  // Check if GPS date data is valid
        date = gps.date.day();    // Get day of the month
        month = gps.date.month(); // Get month
        year = gps.date.year();   // Get year
        
        // Format date as "DD / MM / YYYY"
        date_str = (date < 10 ? "0" + String(date) : String(date)) + " / " +
                   (month < 10 ? "0" + String(month) : String(month)) + " / " +
                   String(year);
      }

      if (gps.time.isValid()) {  // Check if GPS time data is valid
        hour = gps.time.hour();   // Get hour
        minute = gps.time.minute(); // Get minute
        second = gps.time.second(); // Get second
        
        // Convert UTC time to Indian Standard Time (IST)
        minute = (minute + 30) % 60; // Add 30 minutes (IST is UTC +5:30)
        hour = (hour + 5 + (minute >= 30 ? 1 : 0)) % 24; // Add 5 hours
        
        // Determine whether it is AM or PM
        pm = (hour >= 12) ? 1 : 0;
        hour = hour % 12; // Convert to 12-hour format
        
        // Format time as "HH : MM : SS AM/PM"
        time_str = (hour < 10 ? "0" + String(hour) : String(hour)) + " : " +
                   (minute < 10 ? "0" + String(minute) : String(minute)) + " : " +
                   (second < 10 ? "0" + String(second) : String(second)) +
                   (pm ? " PM" : " AM");
      }
    }
  }

  handleClientRequest(); // Handle client request for GPS data
  delay(100);
}

void handleClientRequest() {
  WiFiClient client = server.available(); // Check if a client has connected
  
  if (client) { // If client is connected
    String response = generateHTMLResponse(); // Generate HTML response with GPS data
    client.println(response); // Send response to client
    client.stop(); // Close the connection
  }
}

String generateHTMLResponse() {
  // Generate HTML response with GPS data
  String html = "<!DOCTYPE html><html><head><title>GPS DATA</title>";
  html += "<style>a:link {background-color: yellow; text-decoration: none;}</style>";
  html += "</head><body><h1 style='font-size:300%;text-align:center;'>GPS DATA</h1>";
  html += "<p style='font-size:150%;text-align:center;'><b>Location Details</b></p>";
  html += "<table style='width:50%;margin:auto;text-align:center;'>";
  html += "<tr><th>Latitude</th><td>" + lat_str + "</td></tr>";
  html += "<tr><th>Longitude</th><td>" + lng_str + "</td></tr>";
  html += "<tr><th>Date</th><td>" + date_str + "</td></tr>";
  html += "<tr><th>Time</th><td>" + time_str + "</td></tr>";
  html += "<tr><th>Link</th><td><a href='http://maps.google.com/maps?q=" + lat_str + "+" + lng_str + "'>View on Map</a></td></tr>";
  html += "</table></body></html>";

  return html;
}
