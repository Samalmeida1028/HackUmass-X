

/*
 *Function: httpGETRequest
 * http request from server, takes in the serverName
 */
String httpGETRequest(const char* serverName);


/*
 *Function: httpPOST
 * used to update or send data to the server, has body payload type is plaintext
 */
void httpPOST(const char* serverName, String payload);


/*
 *Function: convertToLED
 * takes in an x and y value of an array and converts it to a string value for the LED matrix
 */
int convertToLED (int x, int y);


/*
 *Function: GetPixelsLit
 * returns the location of all pixels that are not black, and their color value
 */
String GetPixelsLit();

/*
 *Function: GetPixelVal
 * returns the hex value of a pixel (RGB)
 */
String GetPixelVal(int x, int y);

void test();