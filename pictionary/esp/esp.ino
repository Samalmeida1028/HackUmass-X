/*
HackUMass X Project
Sam Almeida, Patrick Walsh, Caleb Scopetski
November 11-13, 2022
*/

#define onoff 0
#define drawerase 2

// Define Global Variables
int pins[4] = {14, 12, 13, 15};
int pinvals[4] = {0};

int onoffbutton = 0; // draw is off initially (toggle)

int drawerasebutton = 1; // set to draw initially (toggle)

// End Global Variables


void setup()
{
  Serial.begin(9600);

  unsigned char i;
  for( i = 0; i < 4; i++) // initializing pins as input
  {
    pinMode(pins[i], INPUT);
  }
  pinMode(onoff, INPUT);
  pinMode(drawerase, INPUT);

  digitalWrite(onoff, HIGH);
  digitalWrite(drawerase, HIGH);
  
}

void loop()
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

  if(!digitalRead(onoff)) // if on off button pressed
  {
    onoffbutton = !onoffbutton;
    Serial.print("\t");
    Serial.print(onoffbutton);
  }
  if(!digitalRead(drawerase)) // if draw erase button pressed
  {
    drawerasebutton = !drawerasebutton;
    Serial.print("\t");
    Serial.print(drawerasebutton);
  }
  
  Serial.print("\n");

  delay(100);
}
