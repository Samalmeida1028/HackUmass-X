

/*
 *Function: httpGETRequest
 * http request from server, takes in the serverName
 */
String httpGET(const char* serverName);


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

/*
 *Function: rst
 * returns the hex value of a pixel (RGB)
 */
void rst();

/*
 *Function: timer
 * resets the LED matrix as a blank screen
 */
void timer();

/*
 *Function: getxdir
 * returns which way x goes according to analog joystick
 */
void getXDir(int val);

/*
 *Function: getydir
 * returns which way y goes according to analog joystick
 */
void getYDir(int val);

/*
 *Function: checklimits
 * function makes sure x and y stay within the limits of the board and don't continue down past the board
 */
void checkLimits();

/*
 *Function: handlepointer
 * handles the pointer on the LED matrix. Controls what the LEDs do around it 
 * (whether they turn on, what color they turn, if they erase, or if they do nothing)
 */
void handlePointer();

/*
 *Function: getButtonInputs
 * returns the hex value of a pixel (RGB)
 */
void getButtonInputs();

/*
 *Function: draw
 * calls most of the functions above. Responsible for drawing everything on the LED matrix. 
 * The lights must only be chosen color once 
 * unless changed color or erased. It also refreshes the LED matrix at the end of its function.
 */
void draw();
