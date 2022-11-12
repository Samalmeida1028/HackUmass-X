#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>

const char* ssid = "NoSignal";
const char* password = "calebpatsam";
const char* serverName = "http://68.183.25.122:3000/test";


void setup() {
  Serial.begin(9600); 

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

void loop() {

  String test = httpGETRequest(serverName);
  delay(500);
  Serial.println(test);
  httpPOST("http://68.183.25.122:3000/matrix", test);
  delay(500);
}



String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);

  // If you need Node-RED/server authentication, insert user and password below
  //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

  // Send HTTP POST request
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
