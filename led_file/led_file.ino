#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
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
  
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() { 
    Serial.begin(9600);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); 
  FastLED.setBrightness(10);
      
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
    test();
  }

  String GetPixelVal(int x, int y){
    long HexRGB = ((long)leds[convertToLED(x,y)].r << 16) | ((long)leds[convertToLED(x,y)].g << 8 ) | (long)leds[convertToLED(x,y)].b; // get value and convert.
    String formatted = String(HexRGB, HEX);
    String zero = "000000";
    if(formatted != "0"){
    int offset = 6-formatted.length();
    int temp = 0;
    while(temp < offset){
      temp++;
    }
    for(int i = temp; i<zero.length();i++){
      zero[i] = formatted[i-temp];
    }
    }
    return zero;
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
  http.POST(payload);
  

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
          if(val!="000000"){
          String temp = "{\"hex\":\"#" + val + "\",\"coord\":{\"x\":\"" + String(i,DEC) + "\",\"y\":\"" + String(15-j,DEC) + "\"}},";
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
      delay(500);
      for(int i = 0; i < 16; i++){
        for(int j = 0; j < 8; j++){
          leds[convertToLED(i, j)] = CRGB::Green; 
          //Serial.println(lit);
          //leds[convertToLED(i, j)] = CRGB::Black;
          //FastLED.show(); 
        }
      }
          FastLED.show(); 
          String lit = GetPixelsLit();
          //httpPOST("http://68.183.25.122:3000/matrix", lit);
}

  
