#include <FastLED.h>
#define DATA_PIN 3
#define NUM_LEDS 26

#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
int clr = 1;

int led_num = 15;

void setup(){
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);

}
void loop(){
  clr = recieveData();
  if (clr == 1) {
    
    for(int i = 1; i <= led_num; i++) {
      leds[x] = CRGB(0,255,0);
    }
    FastLED.show();
    delay(100);
  } else if(clr == 2) {
    for(int i = 1; i <= led_num; i++) {
      leds[x] = CRGB(255,255,0);
    }
    FastLED.show();
    delay(100);
  } else if(clr==3) {
    for(int i = 1; i <= led_num; i++) {
      leds[x] = CRGB(255,0,0);
    }
    FastLED.show();
    delay(100);
  } else {
    Serial.println("Error")
  }
  FastLED.show();
}

int recieveData(){

}