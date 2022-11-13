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

#define while(a) while((yield(),a))
#define FASTLED_ESP8266_D1_PIN_ORDER
#define NUM_LEDS 512
#define DATA_PIN 5
CRGB leds[NUM_LEDS];

const char* ssid = "NoSignal";  
const char* password = "calebpatsam";
const char* serverName = "http://68.183.25.122:3000/start";


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
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
  B00000000, B00110000 };


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
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
  // put your setup code here, to run once:
  
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever

}
}

String start(){
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,5);             // Start at top-left corner
  display.println("WAITING");
  display.display();
  String load = httpGET(serverName);
  return load;
}

void loop() {
  display.clearDisplay();
  String prompt = start();
  Serial.println(prompt);
  while(prompt == "{}"){
  prompt = start();
  }
  Serial.println(prompt);
  display.clearDisplay();
  display.display();
  while(1){
    displayPrompt(prompt);
  }
}

void displayPrompt(String inputString) {
  display.clearDisplay();

  display.setTextSize(3.5);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,5);             // Start at top-left corner
  display.println(inputString);

  display.display();
}

















//#####WIFI FUNCTIONS####

String httpGET(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);


  int httpResponseCode = http.GET();
  

  String payload = "{}"; 

  if (httpResponseCode==200) {
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
