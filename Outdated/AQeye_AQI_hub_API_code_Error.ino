#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

/* This is the code for the main hub, with teh ESP8266
Contains code for the API and placeholder for the radio transmission via the NRF24L01

ERRORS
1. Running gives an Exception(29)
Esp8266 panic'ed

 - Code tries to access protected memory area
*/

//code by Pranit Sanwalka

// Choose two Arduino pins to use for software serial
int RXPin = 3;
int TXPin = 1;

int GPSBaud = 115200;

const char* ssid = "Drill Press Cafe";
const char* password = "passiton";
const char* apiKey = "53b225e53f13075be617b7f1724e22fea76c2058";

const int httpPort = 80;

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin); 
void setup() {
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
}

void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      displayInfo();
    }
  }
  // If 5000 milliseconds pass and there are no characters  in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }

  float lat = gps.location.lat();
  float lng = gps.location.lng();

  WiFiClient Clientobj;
  HTTPClient http;
    
    // Construct the URL for the API request
    String AQIAPI = "http//:aqicn.org/feed/geo::" + String(lat) + ";:" + String(lng) + "/?token=:" + String(apiKey);
    http.begin(Clientobj, AQIAPI);

    int httpResponseCode = http.GET();
    String payload = http.getString();
    Serial.println(payload);
    // transmitdata(payload);

  delay(10000);  // Delay for 10 seconds before making the next request
}

void displayInfo() {
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);

  }
  else
  {
    Serial.println("Location: Not Available");
  }
}

// void transmitdata(payload) {

// }