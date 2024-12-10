//Code with transcievers for the main HUB (Not used in second prototype, explained in documentation)

//Dust Sensor
//MQ-135
//TRANSCIEVERS
//LED
//BUTTON
//BUZZER
//Screen


#include <LiquidCrystal_I2C.h>
#include <RH_NRF24.h>
#include "MQ135.h"
#define ANALOGPIN A2    //  Define Analog PIN on Arduino Board
#define RZERO 206.85    //  Define RZERO Calibration Value
MQ135 gasSensor = MQ135(ANALOGPIN);
 
//TRANSCIEVER
// RH_NRF24 nrf24(8, 7); 
 
// uint8_t color;
//</>


/* Code for the main hub with the GP2Y1010AU0F dust sensor and MQ-135 sensor with wireless radio transmission */


//Code by Pranit Sanwalka


//only LED TEMP
// #include <FastLED.h>
// #define DATA_PIN 15
// #define NUM_LEDS 20

// #define COLOR_ORDER GRB

// CRGB leds[NUM_LEDS];
// int clr = 1;

// int led_num = 15;
//TEMP END

LiquidCrystal_I2C lcd(0x27, 16, 2); //set lcd adress for 16 by 2

//Dust Sensor
int measurePin = A0; //Connect dust sensor to Arduino A0 pin
int ledPower = 14;   //Connect led pin

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
int clrIndicator = 1;
int threshold1 = 100;
int threshold2 = 300;

//Dust Sensor

//MQ
int sensorValue;
int digitalValue;
//MQ


/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground through 220 ohm resistor
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int buzzerpin = ;

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

void setup(){
  Serial.begin(115200);
  pinMode(2, INPUT);
  //Other

  pinMode(ledPower,OUTPUT);
  lcd.init();
  lcd.clear();
  lcd.backlight();

  //button
  pinMode(buzz)
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);



  //transmitting
  int color=1;
  while (!Serial)
    ; // wait for serial port to connect. Needed for Leonardo only
  if (!nrf24.init()) 
  {
    Serial.println("init failed");
 
  }
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(3)) 
  {
    Serial.println("setChannel failed");
  }
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) {
    Serial.println("setRF failed");
  }
  Serial.println("Transmitter started");

  //gas sensor
  float rzero = gasSensor.getRZero();
  delay(3000);
  Serial.print("MQ135 RZERO Calibration Value : ");
  Serial.println(rzero);
  //gas sensor


  // //TEMP
  // FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  // //TEMPEND
}

void loop(){


  //dust sensor
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin); // read the dust value

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); 
  delayMicroseconds(sleepTime);

  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);

  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  dustDensity = 170 * calcVoltage - 0.1;

  //dust sensor end
  float ppm = gasSensor.getPPM();
  delay(1000);
  Serial.print("CO2 ppm value : ");
  Serial.println(ppm);
  lcd.setCursor(0,0);
  lcd.print("   Tech Maker   ");
  lcd.setCursor(0,1);
  lcd.print("CO2 : ");
  lcd.print(ppm);
  lcd.print("PPM");


  //
  Serial.println("Dust/PM2.5 density: " + String(dustDensity) + "ug/m3"); // unit: ug/m3
  lcd.setCursor(1,0);
  lcd.print(String(dustDensity));
  float ppm = gasSensor.getPPM();
  delay(1000);
  Serial.print("CO2 ppm value : ");
  Serial.println(ppm);
  lcd.setCursor(1,1);
  lcd.setCursor(0,1);
  lcd.print("CO2 : ");
  lcd.print(ppm);
  lcd.print("PPM");
  //Colors
  if(dustDensity<threshold1) {    //with aqi, will be: if (dustDensity<threshold)&&(AQI<aqiThreshold)
    Serial.print("Green");
    lcd.setCursor(6,1);
    lcd.print("Green");
    clrIndicator = 1;
  } else if(dustDensity > threshold1 && dustDensity < threshold2) {
    Serial.print("Yellow");
    lcd.setCursor(6,1);
    lcd.print("Yellow");
    clrIndicator = 2;
  } else {
    Serial.print("Red");
    lcd.setCursor(6,1);
    lcd.print("Red");
    clrIndicator = 3;
  }
  // transmitinfo(clrIndicator);

  if clrIndicator = 3 {
    buzzer();
  }
  Serial.println(clrIndicator);
  changecolor(clrIndicator);
  delay(1000);
}

//BUZZER
void buzzer() {
  int pressed = 0
  while (pressed != 1) {
    pinMode(buzzer, HIGH);
      // read the state of the pushbutton value:
    buttonState = digitalRead(buttonPin);

    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH) {
      // turn LED on:
      digitalWrite(buzzerpin, LOW;
      pressed = 1
    }
  }
}

//Transmit Data

// void transmitinfo(int data){
//   Serial.println("Sending to gateway");
//   uint8_t data[2];
//   data[0] = color;
  
//   Serial.println("------------- Measurements -------------");
//   Serial.print("Color: ");
//   Serial.print(data[0]);
//   nrf24.send(data, sizeof(data));
//   nrf24.waitPacketSent();
  
//   // Now wait for a reply
//   uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
//   uint8_t len = sizeof(buf);
 
//   if (nrf24.waitAvailableTimeout(1000))
//   {
//     // Should be a reply message for us now
//     if (nrf24.recv(buf, &len))
//     {
     
//       Serial.print("got reply: ");
//       Serial.println((char*)buf);
//     }
//     else
//     {
     
//       Serial.println("recv failed");
//     }
//   }
//   else
//   {
//     Serial.println("No reply.");
    
//   }
//   delay(5000);
// }



//TEMP
// void changecolor(int clr){
//   if (clr == 1) {
    
//     for(int i = 1; i <= led_num; i++) {
//       leds[i] = CRGB(0,255,0);
//     }
//     FastLED.show();
//     delay(100);
//   } else if(clr == 2) {
//     for(int i = 1; i <= led_num; i++) {
//       leds[i] = CRGB(255,255,0);
//     }
//     FastLED.show();
//     delay(100);
//   } else if(clr==3) {
//     for(int i = 1; i <= led_num; i++) {
//       leds[i] = CRGB(255,0,0);
//     }
//     FastLED.show();
//     delay(100);
//   } else {
//     Serial.println("Error");
//   }
//   FastLED.show();
// }
// //END temp
