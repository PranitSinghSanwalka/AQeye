#include <LiquidCrystal_I2C.h>

/* Code for the main hub with the GP2Y1010AU0F dust sensor and wired leds (temporary code for wired leds)
There is a placeholder for the wireless radio transmission */


//Code by Pranit Sanwalka


//only LED TEMP
#include <FastLED.h>
#define DATA_PIN 15
#define NUM_LEDS 20

#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
int clr = 1;

int led_num = 15;
//TEMP END

LiquidCrystal_I2C lcd(0x27, 16, 2); //set lcd adress for 16 by 2
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
void setup(){
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  lcd.init();
  lcd.clear();
  lcd.backlight();

  //TEMP
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  //TEMPEND
}

void loop(){
  FastLED.show();
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

  
  Serial.println("Dust/PM2.5 density: " + String(dustDensity) + "ug/m3"); // unit: ug/m3
  lcd.setCursor(1,0);
  lcd.print(String(dustDensity));
  if(dustDensity<threshold1) {    //with aqi, will be: if (dustDensity<threshold)&&(AQI<aqiThreshold)
    Serial.print("Green");
    lcd.setCursor(1,1);
    lcd.print("Green");
    clrIndicator = 1;
  } else if(dustDensity > threshold1 && dustDensity < threshold2) {
    Serial.print("Yellow");
    lcd.setCursor(1,1);
    lcd.print("Yellow");
    clrIndicator = 2;
  } else {
    Serial.print("Red");
    lcd.setCursor(1,1);
    lcd.print("Red");
    clrIndicator = 3;
  }
  // transmitinfo(clrIndicator);      //Transmit data
  Serial.println(clrIndicator);
  changecolor(clrIndicator);
  delay(1000);
}


//placeholder for transmission

// void transmitinfo(int data){

// }



//TEMP
void changecolor(int clr){
  if (clr == 1) {
    
    for(int i = 1; i <= led_num; i++) {
      leds[i] = CRGB(0,255,0);
    }
    FastLED.show();
    delay(100);
  } else if(clr == 2) {
    for(int i = 1; i <= led_num; i++) {
      leds[i] = CRGB(255,255,0);
    }
    FastLED.show();
    delay(100);
  } else if(clr==3) {
    for(int i = 1; i <= led_num; i++) {
      leds[i] = CRGB(255,0,0);
    }
    FastLED.show();
    delay(100);
  } else {
    Serial.println("Error");
  }
  FastLED.show();
}
//END temp
