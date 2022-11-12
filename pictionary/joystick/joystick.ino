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
  
  pinvals[3] = 1;

  // Straight down
  if((xval < 100) && (yval < 600 && yval > 400))
  {
    //0
    pinvals[0] = 0;
    pinvals[1] = 0;
    pinvals[2] = 0;
  }
  // Down left
  else if((xval < 100) && (yval < 100))
  {
    // 1
    pinvals[0] = 0;
    pinvals[1] = 0;
    pinvals[2] = 1;
  }
  // left
  else if((xval > 400 && xval < 600) && (yval < 100))
  {
    // 2
    pinvals[0] = 0;
    pinvals[1] = 1;
    pinvals[2] = 0;
  }
  // up left
  else if((xval > 950) && (yval < 100))
  {
    // 3
    pinvals[0] = 0;
    pinvals[1] = 1;
    pinvals[2] = 1;
  }
  // up
  else if((xval > 950) && (yval < 600 && yval > 400))
  {
    // 4
    pinvals[0] = 1;
    pinvals[1] = 0;
    pinvals[2] = 0;
  }
  // up right
  else if((xval > 950) && (yval > 950))
  {
    // 5
    pinvals[0] = 1;
    pinvals[1] = 0;
    pinvals[2] = 1;
  }
  // right
  else if((yval > 950) && (xval < 600 && xval > 400))
  {
    // 6
    pinvals[0] = 1;
    pinvals[1] = 1;
    pinvals[2] = 0;
  }
  // down right
  else if((xval < 100) && (yval > 950))
  {
    // 7
    pinvals[0] = 1;
    pinvals[1] = 1;
    pinvals[2] = 1;
  }
  // nothing pressed
  else if((xval < 600 && xval > 400) && (yval < 600 && yval > 400))
  {
    pinvals[3] = 0; // player not touching joystick
  }

  // printing output to digital pins

  unsigned char i;
  for(i = 0; i < 4; i++)
  {
    if(pinvals[i] == 0)
    {
      digitalWrite(pins[i], LOW);
      Serial.print(0);
    } else if (pinvals[i] == 1)
    {
      digitalWrite(pins[i], HIGH);
      Serial.print(1);
      pinvals[i] = 0; // setting back to 0 for reset
    }
  }
  Serial.print("\n");
  
  
  
  delay(100);
}





  
