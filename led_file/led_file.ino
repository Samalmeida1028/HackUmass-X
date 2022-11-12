#include "FastLED.h"
#define FASTLED_ESP8266_D1_PIN_ORDER
  #define NUM_LEDS 512
  #define DATA_PIN 5
  CRGB leds[NUM_LEDS];
  int speedd = 5;
  
  void setup() { 
    Serial.begin(9600);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); 
  FastLED.setBrightness(10);
  }
  
  void loop() {
      for(int i = 0; i < 32; i++){
        for(int j = 0; j < 16; j++){
          leds[convertToLED(i, j)] = CRGB::White; 
          FastLED.show(); 
          delay(speedd);
          leds[convertToLED(i, j)] = CRGB::Black;
          FastLED.show(); 
        }
      }
  }

  int convertToLED (int x, int y){
    int temp = 8*x;
    if(y > 7){
      temp += 256;
      y%=8;
    }
    if(x%2 == 0){
      temp = temp + y;
    }
    else{
      temp = temp + (7 - y);
    }

    return temp;
  }

  
