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
    WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  unsigned char i;
  for( i = 0; i < 4; i++) // initializing pins as input
  {
    pinMode(pins[i], INPUT);
  }
 
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
  }

  
  
  void loop() {
    test();
  }

  String GetPixelVal(int x, int y){
    long HexRGB = ((long)leds[convertToLED(x,y)].r << 16) | ((long)leds[convertToLED(x,y)].g << 8 ) | (long)leds[convertToLED(x,y)].b; // get value and convert.
    return String(HexRGB, HEX);
  }



String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);


  int httpResponseCode = http.GET();
  

  String payload = "{}"; 

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}


void httpPOST(const char* serverName, String payload) {
  WiFiClient client;
  HTTPClient http;
   

  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  http.addHeader("Content-Type", "text/plain");

  // Send HTTP POST request
  int httpResponseCode = http.POST(payload);
  

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

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

  while(1) // loop for analog joystick to be able to indefinitely move
  {
    unsigned char i;
    for( i = 0; i < 4; i++) // getting input from arduino
    {
      pinvals[i] = digitalRead(pins[i]);
      if(pinvals[i] == HIGH)
      {
        Serial.print(1);
      } else if (pinvals[i] == LOW) {
        Serial.print(0);
      }
    } // for loop
    
    if(pinvals[4] == 0) // if pointer not meant to move
    {
      continue;
    } else if (pinvals[4] == 1){

      dir = pins[0]**2 + pins[1]*2 + pins[0];

      switch(dir) // down right
      {
        case 
      }
      
      
    }
    
    delay(100); // delaying for 0.1 seconds
  }
  
}

  
