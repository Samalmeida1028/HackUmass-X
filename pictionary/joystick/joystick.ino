/*
HackUMass X Project
Sam Almeida, Patrick Walsh, Caleb Scopetski
November 11-13, 2022
*/

// Global Variables
#include "FastLED.h"


CRGB leds[1];
#define FASTLED_ESP8266_D1_PIN_ORDER
#define NUM_LEDS 256
#define DATA_PIN 5
#define joyX A0
#define joyY A1

int pins[4] = {8, 9, 10, 11};

int ledsa[NUM_LEDS];

// declaring 
int xval = -1;
int yval = -1;
int bval;
int j;

int sendtoesp;

char bits[3];

int pinvals[4];


// End Global Variables

char request[] = "link";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
  
  pinMode(pins[0], OUTPUT);
  pinMode(pins[1], OUTPUT);
  pinMode(pins[2], OUTPUT);
  pinMode(pins[3], OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  xval = analogRead(joyX);
  yval = analogRead(joyY);
  Serial.println(xval);
  Serial.println(yval);
  
  pinvals[3] = 1;

  // nothing pressed
  if((xval < 800 && xval > 200) && (yval < 800 && yval > 200))
  {
    pinvals[3] = 0; // player not touching joystick
  }
  // left
  else if((xval < 800 && xval > 200) && (yval < 200))
  {
    // 2
    pinvals[0] = 0;
    pinvals[1] = 1;
    pinvals[2] = 0;
  }
  // Straight down
  else if((xval < 50) && (yval < 530 && yval > 470))
  {
    //0
    pinvals[0] = 0;
    pinvals[1] = 0;
    pinvals[2] = 0;
  }
  // Down left
  else if((xval < 50) && (yval < 50))
  {
    // 1
    pinvals[0] = 0;
    pinvals[1] = 0;
    pinvals[2] = 1;
  }
  // up left
  else if((xval > 1000) && (yval < 50))
  {
    // 3
    pinvals[0] = 0;
    pinvals[1] = 1;
    pinvals[2] = 1;
  }
  // up
  else if((xval > 1000) && (yval < 530 && yval > 470))
  {
    // 4
    pinvals[0] = 1;
    pinvals[1] = 0;
    pinvals[2] = 0;
  }
  // up right
  else if((xval > 1000) && (yval > 1000))
  {
    // 5
    pinvals[0] = 1;
    pinvals[1] = 0;
    pinvals[2] = 1;
  }
  // right
  else if((yval > 1000) && (xval < 700 && xval > 300))
  {
    // 6
    pinvals[0] = 1;
    pinvals[1] = 1;
    pinvals[2] = 0;
  }
  // down right
  else if((xval < 50) && (yval > 1000))
  {
    // 7
    pinvals[0] = 1;
    pinvals[1] = 1;
    pinvals[2] = 1;
  }
  

  // printing output to digital pins

  unsigned char i;
  for(i = 0; i < 4; i++)
  {
    if(pinvals[i] == 0)
    {
      digitalWrite(pins[i], LOW);
    } else if (pinvals[i] == 1)
    {
      digitalWrite(pins[i], HIGH);
      pinvals[i] = 0; // setting back to 0 for reset
    }
  }
  Serial.print("\n");

  delay(100);
}





  
