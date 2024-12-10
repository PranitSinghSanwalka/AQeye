#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <string.h>

const char* ssid = "OnePlus5TSSK";
const char* password = "1234abcd";
const char* apiKey = "53b225e53f13075be617b7f1724e22fea76c2058";

const int httpPort = 80;
float lat = 15.573590;
float lng = 73.826480;

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

}

void loop() {
  WiFiClient clientobj;
  HTTPClient http;
  // Construct the URL for the API request
  String AQIAPI = "http//:aqicn.org/feed/geo::" + String(lat) + ";:" + String(lng) + "/?token=:" + String(apiKey);
  http.begin(clientobj, AQIAPI);

  int httpResponseCode = http.GET();
  String payload = String(http.getString());
  Serial.println(payload);

  delay(10000);  // Delay for 10 seconds before making the next request
}
