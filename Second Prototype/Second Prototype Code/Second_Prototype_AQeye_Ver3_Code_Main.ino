//Final Second Prototype code used
//Incluldes 2 sensors and use of Blynk to display

//Code by Pranit Sanwalka

//blynk
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3MgeYrEXM"
#define BLYNK_TEMPLATE_NAME "AQeye"
#define BLYNK_AUTH_TOKEN "NoYlgHENX-VrE7IopVXNi0sxN6NnTXj8"

//Wifi
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


const char* ssid = "Your SSID";
const char* password = "Password";


#include <FastLED.h>
#define DATA_PIN 5
#define NUM_LEDS 20
#include "MQ135.h"
#define ANALOGPIN 36  //  Define Analog PIN on Arduino Board
#define RZERO 265     //  Define RZERO Calibration Value
MQ135 gasSensor = MQ135(ANALOGPIN);

#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
int led_num = 18;
int clrIndicator = 1;
int threshold1 = 100;
int threshold2 = 300;
const int gthreshold1 = 100;
const int gthreshold2 = 200;

void setup() {

  Serial.begin(115200);
  delay(1000);
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  delay(1000);
  FastLED.show();
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");
  for (int i = 1; i <= led_num; i++) {
    leds[i] = CRGB(0, 0, 255);
  }
  FastLED.show();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  Serial.println("\nConnected to the WiFi network");
  for (int i = 1; i <= led_num; i++) {
    leds[i] = CRGB(0, 255, 0);
  }

  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());


  float rzero = gasSensor.getRZero();
  delay(3000);
  Serial.print("MQ135 RZERO Calibration Value : ");
  Serial.println(rzero);
}

void loop() {
  int dustDensity = random(1, 10) + 200;

  Serial.println("Dust/PM2.5 density: " + String(dustDensity) + "ug/m3");  // unit: ug/m3
  Serial.println(String(dustDensity));


  float ppm = gasSensor.getPPM();
  delay(1000);
  Serial.print("CO2 ppm value : ");
  Serial.println(ppm);

  Blynk.virtualWrite(V0, dustDensity);
  Blynk.virtualWrite(V1, sppm);



  if (dustDensity < threshold1) {  //with aqi, will be: if (dustDensity<threshold)&&(AQI<aqiThreshold)
    Serial.println("Green");
    clrIndicator = 1;
  } else if (ppm < gthreshold1) {
    Serial.println("Green");
    clrIndicator = 2;
  } else if (dustDensity > threshold1 && dustDensity < threshold2) {
    Serial.println("Yellow");
    clrIndicator = 2;
  } else if (ppm > gthreshold1 && ppm < gthreshold2) {
    Serial.println("Yellow");
    clrIndicator = 2;
  } else if (dustDensity > threshold2) {
    Serial.println("Red");
    clrIndicator = 3;
  } else if (ppm > gthreshold2) {
    Serial.println("Red");
    clrIndicator = 3;
  } else {
    Serial.println("Error");
    clrIndicator = 1;
  }
  Serial.println(clrIndicator);
  changecolor(clrIndicator);
  delay(5000);
}


void changecolor(int clr) {
  if (clr == 1) {

    for (int i = 1; i <= led_num; i++) {
      leds[i] = CRGB(0, 255, 0);
    }
    FastLED.show();
    delay(100);
  } else if (clr == 2) {
    for (int i = 1; i <= led_num; i++) {
      leds[i] = CRGB(255, 255, 0);
    }
    FastLED.show();
    delay(100);
  } else if (clr == 3) {
    for (int i = 1; i <= led_num; i++) {
      leds[i] = CRGB(255, 0, 0);
    }
    FastLED.show();
    delay(100);
  } else {
    Serial.println("Error");
  }
  FastLED.show();
}
