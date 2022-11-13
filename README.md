# HackUmass-X
Repository for our HackUmass-X Project!

## Functions and Brief Description

### String httpGETRequest(const char* serverName)
> Requests from server information from the server connected to

### void httpPOST(const char* serverName, String payload)
> Send data to the server 

### int convertToLED (int x, int y)
> the array leds[] is filled with 512 LEDs, in order 0-512. convertToLED() takes input of the x and y coordinates
> of the LED and gets the 0-512 equivalent.

### String GetPixelsLit()
> Sends all LEDs that are lit to the server

### String GetPixelVal(int x, int y)
> Returns the RGB value of the pixel in hexadecimal

### void rst()
> resets the LED matrix as a blank screen

### void timer()
> prints out the timer on the top of the LED matrix

### void getxdir(int val), void getydir(int val)
> taking in the decimal value given by the four digital outputs from the Arduino, getdir 
> takes in that value and has two separate if statements. They either move x up one square or back one
> square, and same with y.

### void checklimits()
> function makes sure x and y stay within the limits of the board and don't continue down
> past the board

### void handlepointer()
> handles the pointer on the LED matrix. Controls what the LEDs do around it (whether they turn on, what 
> color they turn, if they erase, or if they do nothing)

### void draw()
> calls most of the functions above. Responsible for drawing everything on the LED matrix. The lights must only be
> chosen color once unless changed color or erased. It also refreshes the LED matrix at the end of its function.

