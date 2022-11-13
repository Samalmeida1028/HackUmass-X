/*
HackUMass X Project
Sam Almeida, Patrick Walsh, Caleb Scopetski
November 11-13, 2022
*/
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
const char* serverName = "http://68.183.25.122:3000/matrix";

#define onoff 0
#define drawerase 2

// Define Global Variables
int pins[4] = {14, 12, 13, 15};
int pinvals[4] = {0};

int onoffbutton = 0; // draw is off initially (toggle)

int drawerasebutton = 1; // set to draw initially (toggle)

int x = 16;
int y = 8;
int count = 0;
int prevx, prevy;

CRGB leds[NUM_LEDS];

// End Global Variables


void setup()
{
  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); 
  FastLED.setBrightness(10);

  unsigned char k;
  for( k = 0; k < 4; k++) // initializing pins as input
  {
    pinMode(pins[k], INPUT);
  }
  pinMode(onoff, INPUT);
  pinMode(drawerase, INPUT);

  digitalWrite(onoff, HIGH);
  digitalWrite(drawerase, HIGH);
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

void draw(){
  prevx = x;
  prevy = y;
  unsigned char k;
  for( k = 0; k < 4; k++) // getting input from arduino
  {
    pinvals[k] = digitalRead(pins[k]);
  } // for loop

  
  int val = pinvals[0]*4 + pinvals[1]*2 + pinvals[2];
  //Serial.print(val);

  if(!digitalRead(onoff)) // if on off button pressed
  {
    onoffbutton = !onoffbutton;
  }
  if(!digitalRead(drawerase)) // if draw erase button pressed
  {
    drawerasebutton = !drawerasebutton;
  }
  if (pinvals[3] == 1)
  {
    if(val == 7){ // down right
      x = x + 1;
      y = y + 1;}
    else if(val == 6){ // right
      x += 1;}
    else if(val == 5){ // up right
      x += 1;
      y -= 1;}
    else if(val == 4){ // up
      y -= 1;}
    else if(val == 3){ // up left
      x -= 1;
      y -= 1;}
    else if(val == 2){ // left
      x -= 1;}
    else if(val == 1){ // down left
      x -= 1;
      y += 1;}
    else if(val == 0){ // down
      y += 1;}
  }// if statement
  // checking limits
  if(x < 0)
  {
    x = 0;
  }
  else if (x > 31)
  {
    x = 31;
  }
  if(y < 0)
  {
    y = 0;
  }
  else if (y > 15)
  {
    y = 15;
  }
  
  if(onoffbutton && drawerasebutton) // if in draw
  {
    leds[convertToLED(x, y)] = CRGB::White; 
    
  } else if (onoffbutton && !drawerasebutton){ // if erasing
    leds[convertToLED(x, y)] = CRGB::Black; 
    
  } else if (!onoffbutton) { // if user does not want to draw or erase
    leds[convertToLED(x, y)] = CRGB::White;
    if(prevx != x || prevy != y)
    {
      leds[convertToLED(prevx, prevy)] = CRGB::Black;
    }
  } else {
    leds[convertToLED(x, y)] = CRGB::White;
  }
  FastLED.show();
  

  delay(100);
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
  http.POST(payload);
  

  http.end();

}

 String GetPixelsLit(){
        String jsonString = "[";
        
        for(int i = 0; i < 32; i++){
        for(int j = 0; j < 16; j++){
          String val = GetPixelVal(i,j);
          if(val!="0"){
          String temp = "{\"hex\":\"$" + val + "\",\"coord\":{\"x\":\"" + String(i,DEC) + "\",\"y\":\"" + String(15-j,DEC) + "\"}},";
          jsonString += temp;
          }
        }
        }
        int len = jsonString.length()-1;
        jsonString.remove(len);
        jsonString = jsonString + "]";
        return jsonString;
  }



  void loop()
{
  
  draw();
  count++;
  String litLeds = GetPixelsLit();
  if(count>20){
  httpPOST(serverName,litLeds);
  count = 0;
  }
}
