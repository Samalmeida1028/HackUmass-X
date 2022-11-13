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

#define onoff 0
#define drawerase 2

// Define Global Variables
int pins[4] = {14, 12, 13, 15};
int pinvals[4] = {0};

int onoffbutton = 0; // draw is off initially (toggle)

int drawerasebutton = 1; // set to draw initially (toggle)

int x = 16;
int y = 8;

int prevx, prevy;

CRGB leds[NUM_LEDS];
int speedd = 500;

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
  
}

void loop()
{ 
  prevx = x;
  prevy = y;
  unsigned char k;
  for( k = 0; k < 4; k++) // getting input from arduino
  {
    pinvals[k] = digitalRead(pins[k]);
    Serial.print(pinvals[k]);
  } // for loop

  Serial.print("\n");
  
  int val = pinvals[0]*4 + pinvals[1]*2 + pinvals[2];
  //Serial.print(val);

  if(!digitalRead(onoff)) // if on off button pressed
  {
    onoffbutton = !onoffbutton;
    Serial.println(onoffbutton);
  }
  if(!digitalRead(drawerase)) // if draw erase button pressed
  {
    drawerasebutton = !drawerasebutton;
    Serial.println(drawerasebutton);
  }
  if (pinvals[3] == 1)
  {
    if(val == 7){ // down right
      Serial.print(7);
      x = x + 1;
      y = y + 1;}
    else if(val == 6){ // right
      Serial.print(6);
      x += 1;}
    else if(val == 5){ // up right
      Serial.print(5);
      x += 1;
      y -= 1;}
    else if(val == 4){ // up
      Serial.print(4);
      y -= 1;}
    else if(val == 3){ // up left
      Serial.print(3);
      x -= 1;
      y -= 1;}
    else if(val == 2){ // left
      Serial.print(2);
      x -= 1;}
    else if(val == 1){ // down left
      Serial.print(1);
      x -= 1;
      y += 1;}
    else if(val == 0){ // down
      Serial.print(0);
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
  
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\n");
  Serial.print("\n");

  delay(100);
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
