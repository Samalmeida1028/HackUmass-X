# HackUmass-X
Repository for our HackUmass-X Project!

## Join the IP to take a look!
    http://68.183.25.122:3000/
    
## How the Code Works Together

### Hardware

  There are two files that work together to bring Pictionary to life on the client side, which are pictionary_game.ino and joystick.ino.
  Joystick is uploaded on the Arduino Uno and takes the analog input from the joystick and converts the input to four digital outputs, where are sent 
  to D6, D7, D8, and D0 on the ESP8266. 
  
  The pins come meant to be read as three binary inputs made as decimal and a final bit checking if the user is moving the joystick. The three binary inputs start at 0   and make their way around the joystick in a clockwise fashion

  With the analog joystick input sent to the ESP8266, the device then converts that input and alters the x,y values of the cursor based on which direction
  the joystick moved. The surrounding LEDs around the cursor also could be altered dependent on if the user is drawing, and what color the user is drawing with
  (choosing their color at the bottom of the screen). The ESP8266 also emits a timer at the top, a line that gets shorter until it disappears when time runs out.
  All LED indexes that are not black (off) are sent to the server to be interpreted and put on display.

### Server and client side
    The server utilizes websockets to synchronize multiple clients in the game lobby. The websockets update the canvas display on the webpage based on the value of the LED matrix that was POST-ed to the server.
    
    The server tells the hardware when to reset and move to the next game round based on input from the client.
## Functions and Brief Description

### String httpGETRequest(const char* serverName)
  Requests from server information from the server connected to

### void httpPOST(const char* serverName, String payload)
  Send data to the server 

### int convertToLED (int x, int y)
  the array leds[] is filled with 512 LEDs, in order 0-512. convertToLED() takes input of the x and y coordinates
  of the LED and gets the 0-512 equivalent.

### String GetPixelsLit()
  Sends all LEDs that are lit to the server

### String GetPixelVal(int x, int y)
  Returns the RGB value of the pixel in hexadecimal

### void rst()
  resets the LED matrix as a blank screen

### void timer()
  prints out the timer on the top of the LED matrix

### void getxdir(int val), void getydir(int val)
  taking in the decimal value given by the four digital outputs from the Arduino, getdir 
  takes in that value and has two separate if statements. They either move x up one square or back one
  square, and same with y.

### void checklimits()
  function makes sure x and y stay within the limits of the board and don't continue down
  past the board

### void handlepointer()
  handles the pointer on the LED matrix. Controls what the LEDs do around it (whether they turn on, what 
  color they turn, if they erase, or if they do nothing)

### void draw()
  calls most of the functions above. Responsible for drawing everything on the LED matrix. The lights must only be
  chosen color once unless changed color or erased. It also refreshes the LED matrix at the end of its function.

