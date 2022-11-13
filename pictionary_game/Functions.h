

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
String getPixelsLit();

/*
 *Function: GetPixelVal
 * returns the hex value of a pixel (RGB)
 */
String getPixelVal(int x, int y);

void rst();

void timer();

void getxdir(int val);

void getydir(int val);

void checklimits();

void handlepointer();

void getButtonInputs();

/*
 * Function: WaitForPrompt
 * waits for a prompt from the server when starting a game
 */
String waitForPrompt();

/*
 * Function: promptHasChanged
 * checks to see if the prompt has changed to reset the game
 */
bool promptHasChanged(int count, String prompt);


/*
 * Function: getPrompt
 * returns the prompt for the game from the server
 */
String getPrompt();


/*
 * Function: displayPrompt
 * displays a given prompt on the OLED
 */
void displayPrompt(String inputString);