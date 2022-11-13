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
#define DATA_PIN 14
const char* ssid = "NoSignal";  
const char* password = "calebpatsam";
const char* serverName = "http://68.183.25.122:3000/matrix";

#define onoff 0
#define drawerase 2

// Define Global Variables
int pins[4] = {12, 13, 15, 16};
int pinvals[4] = {0};

int onoffbutton = 0; // draw is off initially (toggle)

int drawerasebutton = 1; // set to draw initially (toggle)

int y = 8;
int x = 16;
int count = 0;
int prevx, prevy;

int timeleft = 60;

CRGB leds[NUM_LEDS];

CRGB current = CRGB::White; // current color
CRGB prevcolor = CRGB::Black;

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

  timer();

  CRGB colors[6] = {CRGB::White, CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow, CRGB::Yellow};
  // note: extra yellow added at end as with current conditional to get color would result in error

  
  // adding colors to end of screen
  unsigned char k;
  char l = -1;
  for(k = 0; k < 16; k++)
  {
     if((int) k % 3 == 0)
     {
      l += 1;
     }
     leds[convertToLED(31, k)] = colors[l];
  }
  leds[convertToLED(31, 15)] = colors[l];
  
  prevy = y;
  prevx = x;
  
  for( k = 0; k < 4; k++) // getting input from arduino
  {
    pinvals[k] = digitalRead(pins[k]);
  } // for loop

  
  int val = pinvals[0]*4 + pinvals[1]*2 + pinvals[2];
  //Serial.print(val);

  if(!digitalRead(onoff)) // if on off button pressed
  {
    onoffbutton = !onoffbutton;
    delay(50);
  }
  if(!digitalRead(drawerase)) // if draw erase button pressed
  {
    drawerasebutton = !drawerasebutton;
    delay(50);
  }
  if (pinvals[3] == 1) // if user is moving analog joystick, find new x and y values
  {
    getxdir(val);
    getydir(val);
  }// if statement

  checklimits(); // correcting x and y if they are past thresholds of led matrix

  if(x == 31) // if user is selecting a color
  {
    current = colors[(int) y / 3];
  }
  
  handlepointer();
  FastLED.show();
  

  delay(50);
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

  void rst() // function to reset all squares
  {
    for(int i = 0; i < 32; i++){
      for(int j = 0; j < 16; j++){
        leds[convertToLED(i, j)] = CRGB::Black;
      }
    }
    FastLED.show(); 
  }

  void timer() // prints how much time is left to led matrix
  {
    int val = (int) timeleft / 4;

    unsigned char k;
    for( k = 0; k < val + 1; k++)
    {
      leds[convertToLED(0, 15-k)] = CRGB::White;
    }
    for(k = val + 1; k <= 15; k++)
    {
      leds[convertToLED(0, 15-k)] = CRGB::Black; 
    }
  }
  
  void getxdir(int val) // changing x depending on direction
  {
    if(val == 7 || val == 1 || val == 0)
    {
      x -= 1;
    } else if (val == 5 || val == 4 || val == 3){
      x += 1;
    }
  }

  void getydir(int val) // changes y depending on direction
  {
    if(val == 3 || val == 2 || val == 1)
    {
      y -= 1;
    } else if (val == 7 || val == 6 || val == 5){
      y += 1;
    }
  }

  void checklimits() // making sure x and y are not going past borders
  {
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
  }

  void handlepointer() // function handles pointer if erase, draw, or none of the above
  {
    if(onoffbutton && drawerasebutton) // if in draw
    {
      leds[convertToLED(x, y)] = CRGB::White; 
      leds[convertToLED(prevx, prevy)] = current; 
      
    } else if (onoffbutton && !drawerasebutton){ // if erasing
      leds[convertToLED(x, y)] = CRGB::White; 
      leds[convertToLED(prevx, prevy)] = CRGB::Black; 
      
    } else if (!onoffbutton) { // if user does not want to draw or erase restore previous color behind pointer
      leds[convertToLED(prevx, prevy)] = prevcolor;
      prevcolor = leds[convertToLED(x, y)];
      leds[convertToLED(x, y)] = current;
    }
  }


  void loop()
  {
  
  draw();
  count++;
  if(count>20){
    //char payload[] = httpGETRequest(serverName); // getting how much time is left from server
    timeleft -= 1;
    
    String litLeds = GetPixelsLit();
    httpPOST(serverName,litLeds);
    count = 0;
    }
  }

  
