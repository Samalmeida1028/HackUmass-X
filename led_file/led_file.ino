#include "FastLED.h"
#include "pitches.h"
  #define NUM_LEDS 256
  #define DATA_PIN 5
  CRGB leds[NUM_LEDS];
  const int larray[32][8]= {
    {0,1,2,3,4,5,6,7},{15,14,13,12,11,10,9,8},{16,17,18,19,20,21,22,23},{31,30,29,28,27,26,25,24},
    {32,33,34,35,36,37,38,39},{47,46,45,44,43,42,41,40},{48,49,50,51,52,53,54,55},{63,62,61,60,59,58,57,56},
    {64,65,66,67,68,69,70,71},{79,78,77,76,75,74,73,72},{80,81,82,83,84,85,86,87},{95,94,93,92,91,90,89,88},
    {96,97,98,99,100,101,102,103},{111,110,109,108,107,106,105,104},{112,113,114,115,116,117,118,119},{127,126,125,124,123,122,121,120},
    {128,129,130,131,132,133,134,135},{143,142,141,140,139,138,137,136},{144,145,146,147,148,149,150,151},{159,158,157,156,155,154,153,152},
    {160,161,162,163,164,165,166,167},{175,174,173,172,171,170,169,168},{176,177,178,179,180,181,182,183},{191,190,189,188,187,186,185,184},
    {192,193,194,195,196,197,198,199},{207,206,205,204,203,202,201,200},{208,209,210,211,212,213,214,215},{223,222,221,220,219,218,217,216},
    {224,225,226,227,228,229,230,231},{239,238,237,236,235,234,233,232},{240,241,242,243,244,245,246,247},{255,254,253,252,251,250,249,248}
    
  };
  const int buzzer = 9;
  int speedd = 60;
  int i[]={1,1};
  int num []= {17,4};
  int pnum []= {17,4};
  int score[]={0,0};
  int p1pos = 0;
  int p2pos = 0;
  bool inplay = 1;
  bool canScore = 1;
  bool isStarted = 0;

  const int p1butup = 4;
  int upcheck = 0;
  const int p1butdo = 5;
  int docheck = 0;
  
  const int p2butup = 3;
  int up2check = 0;
  const int p2butdo = 2;
  int do2check = 0;
  
  void setup() { FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); 
  FastLED.setBrightness(10);
  pinMode(p1butup, INPUT);
  pinMode(p1butdo, INPUT);
    pinMode(p2butup, INPUT);
  pinMode(p2butdo, INPUT);
  
  }
  void clear(){
    
      for(int i = 0; i<129; i++){
        leds[i] = CRGB::Magenta;
        leds[256-i] = CRGB::Magenta;FastLED.show();
        leds[i] = CRGB::Black;
        leds[256-i] = CRGB::Black;
      }
  }
  void startGame(){
  i[0]={1};i[1]={1};
  num[0]= {17};num[0]= {17};num[1]= {4};
  score[0]={0};score[1]={0};
  p1pos = 0;
   p2pos = 0;
  inplay = 1;
  canScore = 1;
  speedd = 60;
  clear();
      leds[larray[1][5+p1pos]] = CRGB::Red;FastLED.show(); delay(50);
      leds[larray[1][4+p1pos]] = CRGB::Red;FastLED.show(); delay(50);
      leds[larray[1][3+p1pos]]= CRGB::Red;FastLED.show(); delay(50);
      leds[larray[1][2+p1pos]] = CRGB::Red;FastLED.show(); delay(50);
      leds[larray[30][5+p2pos]] = CRGB::White;FastLED.show(); delay(50);
      leds[larray[30][4+p2pos]] = CRGB::White; FastLED.show(); delay(50);
      leds[larray[30][3+p2pos]]= CRGB::White; FastLED.show(); delay(50);
      leds[larray[30][2+p2pos]] = CRGB::White; FastLED.show(); delay(50);
      tone(buzzer, NOTE_C4); // Send 1KHz sound signal...
      delay(200); 
      tone(buzzer, NOTE_E5); // Send 1KHz sound signal...
      delay(200);
      tone(buzzer, NOTE_C5); // Send 1KHz sound signal...
      delay(200); 
      tone(buzzer, NOTE_C6); // Send 1KHz sound signal...
      delay(600);
      noTone(buzzer);  
      
  }

  void pongAnim(){
    int move = random(0,4);
     if((num[1]>=7||num[1]<=0)||(((num[1]==4||num[1]==3)&&(num[0]==15||num[0]==16))&&move==0)){
      i[1]*=-1;
      tone(buzzer, 100); // Send 1KHz sound signal...
        delay(10);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
    }
    if((num[0]<=0||num[0]>=31)||(((num[1]==4||num[1]==3)&&(num[0]==15||num[0]==16))&&move==1)){
      i[0]*=-1;
      tone(buzzer, 100); // Send 1KHz sound signal...
      delay(10); 
      noTone(buzzer); 
    }
    pnum[0]=num[0];
    num[0]+=i[0];
    pnum[1]=num[1];
    num[1]+=i[1];
    leds[larray[num[0]][num[1]]] = CRGB::White; FastLED.show(); delay(speedd);
    leds[larray[pnum[0]][pnum[1]]] = CRGB::Black;
  }
  
  void loop() {
    while(!isStarted){
      pong();
      pongAnim();
      upcheck = digitalRead(p1butup);
      docheck = digitalRead(p1butdo);
      up2check = digitalRead(p2butup);
      do2check = digitalRead(p2butdo);
      if(upcheck==LOW||docheck==LOW||up2check==LOW||do2check==LOW){
        isStarted = 1;
      }    
    }
    startGame();
      
    while(inplay){
      delay(speedd);
      upcheck = digitalRead(p1butup);
      docheck = digitalRead(p1butdo);
      up2check = digitalRead(p2butup);
      do2check = digitalRead(p2butdo);
      
      if(p1pos<2&&upcheck==LOW){
        p1pos+=1;
        leds[larray[1][2+p1pos-1]] = CRGB::Black;
      }
      
      if (p1pos>-2&&docheck==LOW){
        p1pos-=1;
        leds[larray[1][5+(p1pos+1)]] = CRGB::Black;
      }
      
      if(p2pos<2&&up2check==LOW){
        p2pos+=1;
        leds[larray[30][2+(p2pos-1)]] = CRGB::Black;
      }
      
      if (p2pos>-2&&do2check==LOW){
        p2pos-=1;
         leds[larray[30][5+(p2pos+1)]] = CRGB::Black;
      }
      
      if(score[0]>0){
        leds[larray[14][score[0]-1]] = CRGB::Red;
      }
      
      if(score[1]>0){
        leds[larray[17][score[1]-1]] = CRGB::Blue;
      }
      
      leds[larray[1][5+p1pos]] = CRGB::Red;
      leds[larray[1][4+p1pos]] = CRGB::Red; 
      leds[larray[1][3+p1pos]]= CRGB::Red;
      leds[larray[1][2+p1pos]] = CRGB::Red;
      leds[larray[30][5+p2pos]] = CRGB::Blue;
      leds[larray[30][4+p2pos]] = CRGB::Blue; 
      leds[larray[30][3+p2pos]]= CRGB::Blue;
      leds[larray[30][2+p2pos]] = CRGB::Blue;

    num[0]+=i[0];
    num[1]+=i[1];
    pnum[0]=num[0];
    pnum[1]=num[1];
    leds[larray[pnum[0]][pnum[1]]] = CRGB::Black;
    
    

    
    if(num[1]>=7||num[1]<=0){
      i[1]*=-1;
      tone(buzzer, 10000/speedd); // Send 1KHz sound signal...
        delay(10);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
    }
    
    if((num[0]>=31||(num[0]==30&&(num[1]<=5+p2pos&&num[1]>=2+p2pos))) || ((num[0]<=0)||(num[0]==1&&(num[1]<=5+p1pos&&num[1]>=2+p1pos)))){
      i[0]*=-1;
      tone(buzzer, 1000); // Send 1KHz sound signal...
      delay(10); 
      noTone(buzzer); 
      if(speedd>10){
      speedd-=6;
      }
      
    if(num[0]==31&&canScore){
      leds[larray[pnum[0]][pnum[1]]] = CRGB::Black;
      leds[larray[num[0]][num[1]]] = CRGB::Black;
       num[0]=16;
       num[1]=random(1,6);
      score[0]+=1;
      leds[larray[14][score[0]-2]] = CRGB::Black;
      canScore = 0;
      speedd=60;
      tone(buzzer, 800); // Send 1KHz sound signal...
      delay(100); 
      tone(buzzer, 1000); // Send 1KHz sound signal...
      delay(50); 
      noTone(buzzer); 
      i[0]*=-1;
    }
    
    if(num[0]==0&&canScore){
      leds[larray[pnum[0]][pnum[1]]] = CRGB::Black;
      leds[larray[num[0]][num[1]]] = CRGB::Black;
       num[0]=14;
       num[1]=random(1,6);
      score[1]+=1;
      leds[larray[17][score[1]-2]] = CRGB::Black;
      canScore = 0; 
      speedd=60;
      tone(buzzer, 800); // Send 1KHz sound signal...
      delay(100); 
      tone(buzzer, 1000); // Send 1KHz sound signal...
      delay(50); 
      noTone(buzzer); 
      i[0]*=-1;
       
      
    }
    }

    
    if(num[0]==15){
      canScore = 1;
    }

      leds[larray[num[0]][num[1]]] = CRGB::White; FastLED.show(); delay(10);
      leds[larray[pnum[0]][pnum[1]]] = CRGB::Black;
 
  if(score[0]==8||score[1]==8){
    inplay=0;
    if(score[0]==8){
    p1();
    }
    else{
    p2();
    
    }
    delay(1000);
    wins();
      tone(buzzer, NOTE_C3); // Send 1KHz sound signal...
      delay(200); 
      tone(buzzer, NOTE_E4); // Send 1KHz sound signal...
      delay(200);
      tone(buzzer, NOTE_C5); // Send 1KHz sound signal...
      delay(200); 
      tone(buzzer, NOTE_C6); // Send 1KHz sound signal...
      delay(600);
      tone(buzzer, NOTE_C5); // Send 1KHz sound signal...
      delay(200); 
      tone(buzzer, NOTE_E4); // Send 1KHz sound signal...
      delay(200);
      tone(buzzer, NOTE_C3); // Send 1KHz sound signal...
      delay(200); 
      noTone(buzzer); 
  }
  
  }
  isStarted = 0;
  delay(5000);
  clear();
  }



  
void p1(){
leds[larray[1][0]] = CRGB::Red;
leds[larray[1][1]] = CRGB::Red;
leds[larray[1][2]] = CRGB::Red;
leds[larray[1][3]] = CRGB::Red;
leds[larray[1][4]] = CRGB::Red;
leds[larray[1][5]] = CRGB::Red;
leds[larray[1][6]] = CRGB::Red;
leds[larray[1][7]] = CRGB::Red;
leds[larray[2][0]] = CRGB::Red;
leds[larray[2][4]] = CRGB::Red;
leds[larray[3][0]] = CRGB::Red;
leds[larray[3][4]] = CRGB::Red;
leds[larray[4][0]] = CRGB::Red;
leds[larray[4][4]] = CRGB::Red;
leds[larray[5][0]] = CRGB::Red;
leds[larray[5][1]] = CRGB::Red;
leds[larray[5][2]] = CRGB::Red;
leds[larray[5][3]] = CRGB::Red;
leds[larray[5][4]] = CRGB::Red;
leds[larray[9][1]] = CRGB::Red;
leds[larray[8][2]] = CRGB::Red;
leds[larray[10][0]] = CRGB::Red;
leds[larray[10][1]] = CRGB::Red;
leds[larray[10][2]] = CRGB::Red;
leds[larray[10][3]] = CRGB::Red;
leds[larray[10][4]] = CRGB::Red;
leds[larray[10][5]] = CRGB::Red;
leds[larray[10][6]] = CRGB::Red;
leds[larray[10][7]] = CRGB::Red; 
leds[larray[9][7]] = CRGB::Red; 
leds[larray[8][7]] = CRGB::Red; 
leds[larray[11][7]] = CRGB::Red; 
leds[larray[12][7]] = CRGB::Red; 


FastLED.show();





  
}




void p2(){
leds[larray[1][0]] = CRGB::Blue;
leds[larray[1][1]] = CRGB::Blue;
leds[larray[1][2]] = CRGB::Blue;
leds[larray[1][3]] = CRGB::Blue;
leds[larray[1][4]] = CRGB::Blue;
leds[larray[1][5]] = CRGB::Blue;
leds[larray[1][6]] = CRGB::Blue;
leds[larray[1][7]] = CRGB::Blue;
leds[larray[2][0]] = CRGB::Blue;
leds[larray[2][4]] = CRGB::Blue;
leds[larray[3][0]] = CRGB::Blue;
leds[larray[3][4]] = CRGB::Blue;
leds[larray[4][0]] = CRGB::Blue;
leds[larray[4][4]] = CRGB::Blue;
leds[larray[5][0]] = CRGB::Blue;
leds[larray[5][1]] = CRGB::Blue;
leds[larray[5][2]] = CRGB::Blue;
leds[larray[5][3]] = CRGB::Blue;
leds[larray[5][4]] = CRGB::Blue;
leds[larray[8][0]] = CRGB::Blue;
leds[larray[8][4]] = CRGB::Blue;
leds[larray[8][5]] = CRGB::Blue;
leds[larray[8][6]] = CRGB::Blue;
leds[larray[8][7]] = CRGB::Blue;
leds[larray[9][0]] = CRGB::Blue;
leds[larray[10][0]] = CRGB::Blue;
leds[larray[11][0]] = CRGB::Blue;
leds[larray[12][0]] = CRGB::Blue;
leds[larray[12][1]] = CRGB::Blue;
leds[larray[12][2]] = CRGB::Blue;
leds[larray[12][3]] = CRGB::Blue;
leds[larray[12][4]] = CRGB::Blue;
leds[larray[11][4]] = CRGB::Blue;
leds[larray[10][4]] = CRGB::Blue;
leds[larray[9][4]] = CRGB::Blue;
leds[larray[9][7]] = CRGB::Blue;
leds[larray[10][7]] = CRGB::Blue;
leds[larray[11][7]] = CRGB::Blue;
leds[larray[12][7]] = CRGB::Blue; FastLED.show(); 
}



void wins(){
leds[larray[15][0]] = CRGB::Red;
leds[larray[15][1]] = CRGB::Red;
leds[larray[15][2]] = CRGB::Red;
leds[larray[15][3]] = CRGB::Red;
leds[larray[15][4]] = CRGB::Red;
leds[larray[15][5]] = CRGB::Red;
leds[larray[15][6]] = CRGB::Red;
leds[larray[15][7]] = CRGB::Red;
leds[larray[16][7]] = CRGB::Red;
leds[larray[17][0]] = CRGB::Red;
leds[larray[17][1]] = CRGB::Red;
leds[larray[17][2]] = CRGB::Red;
leds[larray[17][3]] = CRGB::Red;
leds[larray[17][4]] = CRGB::Red;
leds[larray[17][5]] = CRGB::Red;
leds[larray[17][6]] = CRGB::Red;
leds[larray[17][7]] = CRGB::Red;
leds[larray[19][0]] = CRGB::Red;
leds[larray[19][1]] = CRGB::Red;
leds[larray[19][2]] = CRGB::Red;
leds[larray[19][3]] = CRGB::Red;
leds[larray[19][4]] = CRGB::Red;
leds[larray[19][5]] = CRGB::Red;
leds[larray[19][6]] = CRGB::Red;
leds[larray[19][7]] = CRGB::Red;
leds[larray[18][7]] = CRGB::Red;

leds[larray[21][0]] = CRGB::Red;
leds[larray[21][1]] = CRGB::Red;
leds[larray[21][2]] = CRGB::Red;
leds[larray[21][3]] = CRGB::Red;
leds[larray[21][4]] = CRGB::Red;
leds[larray[21][5]] = CRGB::Red;
leds[larray[21][6]] = CRGB::Red;
leds[larray[21][7]] = CRGB::Red;

leds[larray[23][0]] = CRGB::Red;
leds[larray[23][1]] = CRGB::Red;
leds[larray[23][2]] = CRGB::Red;
leds[larray[23][3]] = CRGB::Red;
leds[larray[23][4]] = CRGB::Red;
leds[larray[23][5]] = CRGB::Red;
leds[larray[23][6]] = CRGB::Red;
leds[larray[23][7]] = CRGB::Red;
leds[larray[24][1]] = CRGB::Red;
leds[larray[25][2]] = CRGB::Red;
leds[larray[25][3]] = CRGB::Red;
leds[larray[25][4]] = CRGB::Red;
leds[larray[25][5]] = CRGB::Red;
leds[larray[26][6]] = CRGB::Red;
leds[larray[27][0]] = CRGB::Red;
leds[larray[27][1]] = CRGB::Red;
leds[larray[27][2]] = CRGB::Red;
leds[larray[27][3]] = CRGB::Red;
leds[larray[27][4]] = CRGB::Red;
leds[larray[27][5]] = CRGB::Red;
leds[larray[27][6]] = CRGB::Red;
leds[larray[27][7]] = CRGB::Red;


leds[larray[29][0]] = CRGB::Red;
leds[larray[30][0]] = CRGB::Red;
leds[larray[31][0]] = CRGB::Red;
leds[larray[29][4]] = CRGB::Red;
leds[larray[30][4]] = CRGB::Red;
leds[larray[31][4]] = CRGB::Red;
leds[larray[29][7]] = CRGB::Red;
leds[larray[30][7]] = CRGB::Red;
leds[larray[31][7]] = CRGB::Red;
leds[larray[29][1]] = CRGB::Red;
leds[larray[29][2]] = CRGB::Red;
leds[larray[29][3]] = CRGB::Red;
leds[larray[31][6]] = CRGB::Red;
leds[larray[31][5]] = CRGB::Red; FastLED.show(); 
}



void pong(){

leds[larray[1][0]] = CRGB::Red;
leds[larray[1][1]] = CRGB::Red;
leds[larray[1][2]] = CRGB::Red;
leds[larray[1][3]] = CRGB::Red;
leds[larray[1][4]] = CRGB::Red;
leds[larray[1][5]] = CRGB::Red;
leds[larray[1][6]] = CRGB::Red;
leds[larray[1][7]] = CRGB::Red;
leds[larray[2][0]] = CRGB::Red;
leds[larray[2][4]] = CRGB::Red;
leds[larray[3][0]] = CRGB::Red;
leds[larray[3][4]] = CRGB::Red;
leds[larray[4][0]] = CRGB::Red;
leds[larray[4][4]] = CRGB::Red;
leds[larray[5][0]] = CRGB::Red;
leds[larray[5][1]] = CRGB::Red;
leds[larray[5][2]] = CRGB::Red;
leds[larray[5][3]] = CRGB::Red;
leds[larray[5][4]] = CRGB::Red;


leds[larray[8][0]] = CRGB::Red;
leds[larray[8][1]] = CRGB::Red;
leds[larray[8][2]] = CRGB::Red;
leds[larray[8][3]] = CRGB::Red;
leds[larray[8][4]] = CRGB::Red;
leds[larray[8][5]] = CRGB::Red;
leds[larray[8][6]] = CRGB::Red;
leds[larray[8][7]] = CRGB::Red;
leds[larray[9][0]] = CRGB::Red;
leds[larray[9][7]] = CRGB::Red;
leds[larray[10][0]] = CRGB::Red;
leds[larray[10][7]] = CRGB::Red;
leds[larray[11][0]] = CRGB::Red;
leds[larray[11][7]] = CRGB::Red;
leds[larray[12][0]] = CRGB::Red;
leds[larray[12][1]] = CRGB::Red;
leds[larray[12][2]] = CRGB::Red;
leds[larray[12][3]] = CRGB::Red;
leds[larray[12][4]] = CRGB::Red;
leds[larray[12][5]] = CRGB::Red;
leds[larray[12][6]] = CRGB::Red;
leds[larray[12][7]] = CRGB::Red; 

leds[larray[19][0]] = CRGB::Red;
leds[larray[19][1]] = CRGB::Red;
leds[larray[19][2]] = CRGB::Red;
leds[larray[19][3]] = CRGB::Red;
leds[larray[19][4]] = CRGB::Red;
leds[larray[19][5]] = CRGB::Red;
leds[larray[19][6]] = CRGB::Red;
leds[larray[19][7]] = CRGB::Red;
leds[larray[20][1]] = CRGB::Red;
leds[larray[21][2]] = CRGB::Red;
leds[larray[21][3]] = CRGB::Red;
leds[larray[21][4]] = CRGB::Red;
leds[larray[21][5]] = CRGB::Red;
leds[larray[22][6]] = CRGB::Red;
leds[larray[23][0]] = CRGB::Red;
leds[larray[23][1]] = CRGB::Red;
leds[larray[23][2]] = CRGB::Red;
leds[larray[23][3]] = CRGB::Red;
leds[larray[23][4]] = CRGB::Red;
leds[larray[23][5]] = CRGB::Red;
leds[larray[23][6]] = CRGB::Red;
leds[larray[23][7]] = CRGB::Red;

leds[larray[26][0]] = CRGB::Red;
leds[larray[27][0]] = CRGB::Red;
leds[larray[28][0]] = CRGB::Red;
leds[larray[29][0]] = CRGB::Red;
leds[larray[30][0]] = CRGB::Red;
leds[larray[30][4]] = CRGB::Red;
leds[larray[30][5]] = CRGB::Red; 
leds[larray[30][6]] = CRGB::Red;
leds[larray[29][4]] = CRGB::Red;
leds[larray[26][7]] = CRGB::Red; 
leds[larray[27][7]] = CRGB::Red;
leds[larray[28][7]] = CRGB::Red;
leds[larray[29][7]] = CRGB::Red;
leds[larray[30][7]] = CRGB::Red; 
leds[larray[26][1]] = CRGB::Red;
leds[larray[26][2]] = CRGB::Red;
leds[larray[26][3]] = CRGB::Red;
leds[larray[26][4]] = CRGB::Red;
leds[larray[26][5]] = CRGB::Red; 
leds[larray[26][6]] = CRGB::Red;
leds[larray[26][7]] = CRGB::Red; 

leds[larray[16][4]] = CRGB::White; 
leds[larray[16][3]] = CRGB::White; 
leds[larray[15][4]] = CRGB::White; 
leds[larray[15][3]] = CRGB::White; 


FastLED.show(); 
}
