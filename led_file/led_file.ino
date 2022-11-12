#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include "FastLED.h"
#define FASTLED_ESP8266_D1_PIN_ORDER
#define NUM_LEDS 512
#define DATA_PIN 5
const char* ssid = "NoSignal";
const char* password = "calebpatsam";
const char* serverName = "http://68.183.25.122:3000/test";

CRGB leds[NUM_LEDS];
int speedd = 500;
  
  void setup() { 
    Serial.begin(9600);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); 
  FastLED.setBrightness(10);
    WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
  }

  
  
  void loop() {
      String test = httpGETRequest(serverName);
       delay(500);
       Serial.println(test);
      for(int i = 0; i < 32; i++){
        for(int j = 0; j < 16; j++){
          leds[convertToLED(i, j)] = CRGB::White; 
          FastLED.show(); 
          delay(speedd);
          String lit = GetPixelsLit();
          httpPOST("http://68.183.25.122:3000/matrix", lit);
          Serial.println(lit);
          leds[convertToLED(i, j)] = CRGB::Black;
          FastLED.show(); 
        }
      }
  }

  String GetPixelVal(int x, int y){
    long HexRGB = ((long)leds[convertToLED(x,y)].r << 16) | ((long)leds[convertToLED(x,y)].g << 8 ) | (long)leds[convertToLED(x,y)].b; // get value and convert.
    return String(HexRGB, HEX);
  }

//{{hex:"HEXNUMBER", coord: {X:x,Y:y}},...,n}
//
//





  
