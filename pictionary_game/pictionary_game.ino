#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include "FastLED.h"
#include "Functions.h"

#define while(a) while((yield(), a))
#define FASTLED_ESP8266_D1_PIN_ORDER
#define NUM_LEDS 512
#define DATA_PIN 14
CRGB leds[NUM_LEDS];

const char *ssid = "NoSignal";
const char *password = "calebpatsam";
const char *serverName = "http://68.183.25.122:3000/start";

#define onoff 0
#define drawerase 2
#define rstbuttonpin 9

// Define Global Variables
int pins[4] = {12, 13, 15, 16};
int pinvals[4] = {0};

String lastPrompt = "{}";

int onoffbutton = 0; // draw is off initially (toggle)

int drawerasebutton = 1; // set to draw initially (toggle)

int rstbutton = 0;

int y = 8;
int x = 16;
int count = 0;
int prevx, prevy;

int timeleft = 60;

CRGB current = CRGB::White; // current color
CRGB prevcolor = CRGB::Black;

// End Global Variables

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const unsigned char PROGMEM logo_bmp[] =
    {B00000000, B11000000,
     B00000001, B11000000,
     B00000001, B11000000,
     B00000011, B11100000,
     B11110011, B11100000,
     B11111110, B11111000,
     B01111110, B11111111,
     B00110011, B10011111,
     B00011111, B11111100,
     B00001101, B01110000,
     B00011011, B10100000,
     B00111111, B11100000,
     B00111111, B11110000,
     B01111100, B11110000,
     B01110000, B01110000,
     B00000000, B00110000};

void setup() // sets up for wifi, fastLED, and OLED
{
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(10);
  WiFi.begin(ssid, password);

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.display();
  
  // initializing all necessary pins
  unsigned char k;
  for( k = 0; k < 4; k++) // initializing pins as input
  {
    pinMode(pins[k], INPUT);
  }
  pinMode(onoff, INPUT);
  pinMode(drawerase, INPUT);
  pinMode(rstbuttonpin, INPUT);

  // internal pullup resistor for buttons
  digitalWrite(onoff, HIGH);
  digitalWrite(drawerase, HIGH);
  digitalWrite(rstbuttonpin, HIGH);
}




bool promptHasChanged(String prompt){
  return prompt != httpGET(serverName);
  
}
void loop() {
  String prompt = waitForPrompt();
  displayPrompt(prompt);
  // jay told me to do this
  int count = 0;
  bool p = 0;
  while(!p){ // 
    draw();
    getButtonInputs();
    count++;  
    if(count%40==0){
      String litLeds = getPixelsLit();
      httpPOST("http://68.183.25.122:3000/matrix",litLeds);  
    }
    if(count > 80 ){
      count = 0;
      timeleft -= 4;
      p = promptHasChanged(prompt);
    }
  }
}


//#####WIFI FUNCTIONS####

String httpGET(const char *serverName) //gets data from the servers
{
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode == 200)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    payload = "{}";
  }
  // Free resources
  http.end();

  return payload;
}

void httpPOST(const char *serverName, String payload)
{
  WiFiClient client;
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(client, serverName);
  http.addHeader("Content-Type", "text/plain");

  // Send HTTP POST request
  http.POST(payload);

  http.end();
}

//###DRAWING FUNCTIONS###

void draw()
{
  timer();

  CRGB colors[6] = {CRGB::White, CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow, CRGB::Yellow};
  // note: extra yellow added at end as with current conditional to get color would result in error

  // adding colors to end of screen
  unsigned char k;
  char l = -1;
  for (k = 0; k < 16; k++)
  {
    if ((int)k % 3 == 0)
    {
      l += 1;
    }
    leds[convertToLED(31, k)] = colors[l];
  }
  leds[convertToLED(31, 15)] = colors[l]; // adding final box above loop didn't get

  prevy = y;
  prevx = x;

  for (k = 0; k < 4; k++) // getting input from arduino
  {
    pinvals[k] = digitalRead(pins[k]);
  } // for loop

  int val = pinvals[0] * 4 + pinvals[1] * 2 + pinvals[2];
  // Serial.print(val);

  if (pinvals[3] == 1) // if user is moving analog joystick, find new x and y values
  {
    getXDir(val);
    getYDir(val);
  } // if statement

  checkLimits(); // correcting x and y if they are past thresholds of led matrix

  if (x == 31) // if user is selecting a color
  {
    current = colors[(int)y / 3];
  }

  handlePointer(); // printing cursor and affecting LEDs around it
  FastLED.show();

  delay(35);
}

void checkLimits() // making sure x and y are not going past borders
{
  if (x < 0)
  {
    x = 0;
  }
  else if (x > 31)
  {
    x = 31;
  }
  if (y < 0)
  {
    y = 0;
  }
  else if (y > 15)
  {
    y = 15;
  }
}

void handlePointer() // function handles pointer if erase, draw, or none of the above
{
  if (onoffbutton && drawerasebutton) // if in draw
  {
    leds[convertToLED(x, y)] = CRGB::White;
    if (prevx != x || prevy != y) // making sure to not overwrite white
    {
      leds[convertToLED(prevx, prevy)] = current;
    }
  }
  else if (onoffbutton && !drawerasebutton)
  { // if erasing
    leds[convertToLED(x, y)] = CRGB::Magenta;
    if (prevx != x || prevy != y) // making sure to not overwrite magenta
    {
      leds[convertToLED(prevx, prevy)] = CRGB::Black;
    }
  }
  else if (!onoffbutton)
  { // if user does not want to draw or erase restore previous color behind pointer
    leds[convertToLED(prevx, prevy)] = prevcolor;
    prevcolor = leds[convertToLED(x, y)];
    leds[convertToLED(x, y)] = current;
  }
}

void getButtonInputs() // get buttons if user presses them
{
  if (!digitalRead(onoff)) // if on off button pressed
  {
    onoffbutton = !onoffbutton;
    delay(200);
  }
  if (!digitalRead(drawerase)) // if draw erase button pressed
  {
    drawerasebutton = !drawerasebutton;
    delay(200);
  }
  if (!digitalRead(rstbuttonpin))
  {
    rst();
  }
}

//##Conversion Functions##

int convertToLED(int x, int y) // converts a matrix dimension to the series of the led
{
  int temp = 8 * x;
  if (y > 7)
  {
    temp += 256;
    y %= 8;
  }
  if (x % 2 == 0)
  {
    temp = temp + y;
  }
  else
  {
    temp = temp + (7 - y);
  }

  return temp;
}

String getPixelVal(int x, int y) // gets rgb value in hex #RRGGBB
{
  long HexRGB = ((long)leds[convertToLED(x, y)].r << 16) | ((long)leds[convertToLED(x, y)].g << 8) | (long)leds[convertToLED(x, y)].b; // get value and convert.
  String formatted = String(HexRGB, HEX);
  String zero = "000000";
  if (formatted != "0")
  {
    int offset = 6 - formatted.length(); // formats pixel to add leading zeros
    int temp = 0;
    while (temp < offset)
    {
      temp++;
    }
    for (int i = temp; i < zero.length(); i++)
    {
      zero[i] = formatted[i - temp];
    }
  }
  return zero;
}

String getPixelsLit() // returns all lit pixels in the graph
{
  String jsonString = "[";

  for (int i = 0; i < 32; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      String val = getPixelVal(i, j);
      if (val != "000000")
      {
        String temp = "{\"hex\":\"#" + val + "\",\"coord\":{\"x\":\"" + String(i, DEC) + "\",\"y\":\"" + String(15 - j, DEC) + "\"}},";
        jsonString += temp;
      }
    }
  }
  int len = jsonString.length() - 1;
  jsonString.remove(len);
  jsonString = jsonString + "]";
  return jsonString;
}

//##MISC FUNCTIONS##


String waitForPrompt()
{
  while (getPrompt() == "{}")// while prompt is equal to the original string in the http get request wait
  {
    displayPrompt("WAITING");// displays waiting on the OLED
    delay(100);
  };
  return getPrompt(); // gets prompt from the 
}


bool promptHasChanged(int count, String prompt) 
{
  if (count == 50) //only checks every so often
  {
    Serial.println(httpGET(serverName));
    return prompt != httpGET(serverName);
  }
  return 0;
}

String getPrompt() // receives a prompt from the server
{
  String load = httpGET(serverName);
  return load;
}


void displayPrompt(String inputString)
{
  display.clearDisplay();

  display.setTextSize(3.5);    // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 5);     // Start at top-left corner
  display.println(inputString);

  display.display();
}

void rst() // function to reset all squares
{
  for(int i = 0; i < 32; i++){
    for(int j = 0; j < 16; j++){
      leds[convertToLED(i, j)] = CRGB::Black;
    }
  }
  timeleft = 60;
}

void timer() // prints how much time is left to led matrix
{
  int val = (int)timeleft / 4;

  unsigned char k;
  for (k = 0; k < val + 1; k++)
  {
    leds[convertToLED(0, 15 - k)] = CRGB::White;
  }
  for (k = val + 1; k <= 15; k++)
  {
    leds[convertToLED(0, 15 - k)] = CRGB::Black;
  }
}

void getXDir(int val) // changing x depending on direction
{
  if (val == 7 || val == 1 || val == 0)
  {
    x -= 1;
  }
  else if (val == 5 || val == 4 || val == 3)
  {
    x += 1;
  }
}

void getYDir(int val) // changes y depending on direction
{
  if (val == 3 || val == 2 || val == 1)
  {
    y -= 1;
  }
  else if (val == 7 || val == 6 || val == 5)
  {
    y += 1;
  }
}
