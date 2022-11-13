#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include "Functions.h"
#include "FastLED.h"
#define FASTLED_ESP8266_D1_PIN_ORDER
#define NUM_LEDS 512
#define DATA_PIN 5
const char* ssid = "NoSignal";  
const char* password = "calebpatsam";
const char* serverName = "http://68.183.25.122:3000/test";

CRGB leds[NUM_LEDS];
int speedd = 500;


// controls for analog joystick, buttons
int pins[4] = {14, 12, 13, 15};
int pinvals[4] = {0};
int dir = 0;
  
  
void setup() { 
    Serial.begin(9600);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); 
  FastLED.setBrightness(10);

  unsigned char i;
  for( i = 0; i < 4; i++) // initializing pins as input
  {
    pinMode(pins[i], INPUT);
  }
 }

  void test()
  {
  
        //String test = httpGETRequest(serverName);
        //Serial.println(test);
  //      for(int i = 0; i < 32; i++){
  //        for(int j = 0; j < 16; j++){
  //          leds[convertToLED(i, j)] = CRGB::White; 
  //          FastLED.show(); 
  //          delay(speedd/10);
  //          //String lit = GetPixelsLit();
  //          //httpPOST("http://68.183.25.122:3000/matrix", lit);
  //          Serial.println(convertToLED(i, j));
  //          //leds[convertToLED(i, j)] = CRGB::Black;
  //          //FastLED.show(); 
  //
  //          
  //          
  //        }
  //      }
  
    int i = 16; // x coordinate
    int j = 8; // y coordinate
  
    unsigned char k;
    for( k = 0; k < 4; i++) // getting input from arduino
    {
      pinvals[k] = digitalRead(pins[k]);
    } // for loop
    
    if(pinvals[4] == 0) // if pointer not meant to move
    {
      return;
    } else if (pinvals[4] == 1){
  
      dir = pins[0]*4 + pins[1]*2 + pins[0];
      Serial.println(dir);
  
//      switch(dir)
//      {
//        case 7: // down right
//          i += 1;
//          j += 1;
//        case 6: // right
//          i += 1;
//        case 5: // up right
//          i += 1;
//          j -= 1;
//        case 4: // up
//          j -= 1;
//        case 3: // up left
//          i -= 1;
//          j -= 1;
//        case 2: // left
//          i -= 1;
//        case 1: // down left
//          i -= 1;
//          j += 1;
//        case 0: // down
//          j += 1;
//      }
//      leds[convertToLED(i, j)] = CRGB::White; 
//      FastLED.show(); 
//      Serial.print(i);
//      Serial.print("\t");
//      Serial.print(j);
//      Serial.print("\n");
    
      } // else if pinvals[4] == 1
      delay(200);
  } // void test()

  
  
  void loop() {
    Serial.println("Hello world!");
    test();
  }

  String GetPixelVal(int x, int y){
    long HexRGB = ((long)leds[convertToLED(x,y)].r << 16) | ((long)leds[convertToLED(x,y)].g << 8 ) | (long)leds[convertToLED(x,y)].b; // get value and convert.
    return String(HexRGB, HEX);
  }

 int convertToLED (int x, int y){
    int temp = 8*x;
    if(y > 7){
      temp += 256;
      y%=8;
    }
    if(x%2 == 0){
      temp = temp +  y;
    }
    else{
      temp = temp + (7-y);
    }

    return temp;
  }

    String GetPixelsLit(){
        String jsonString = "[";
        
        for(int i = 0; i < 32; i++){
        for(int j = 0; j < 16; j++){
          String val = GetPixelVal(i,j);
          if(val!="0"){
          String temp = "{hex:" + val + ",coord:{x:" + String(i,DEC) + ",y:" + String(j,DEC) + "}},";
          jsonString += temp;
          }
        }
        }
        int len = jsonString.length()-1;
        jsonString.remove(len);
        jsonString = jsonString + "]";
        return jsonString;
  }
