

String httpGETRequest(const char* serverName);

void httpPOST(const char* serverName, String payload);

int convertToLED (int x, int y);

String GetPixelsLit();

String GetPixelVal(int x, int y);

void rst();

void timer();

void getxdir(int val);

void getydir(int val);

void checklimits();

void handlepointer();
