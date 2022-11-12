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
  http.addHeader("Content-Type", "text/plain");

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
