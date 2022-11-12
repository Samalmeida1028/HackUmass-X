  int convertToLED (int x, int y){
    int temp = 8*x;
    if(y > 7){
      temp += 256;
      y%=8;
    }
    if(x%2 == 0){
      temp = temp +  y;
    }
    else{
      temp = temp + (7-y);
    }

    return temp;
  }

    String GetPixelsLit(){
        String jsonString = "[";
        
        for(int i = 0; i < 32; i++){
        for(int j = 0; j < 16; j++){
          String val = GetPixelVal(i,j);
          if(val!="0"){
          String temp = "{hex:" + val + ",coord:{x:" + String(i,DEC) + ",y:" + String(j,DEC) + "}},";
          jsonString += temp;
          }
        }
        }
        int len = jsonString.length()-1;
        jsonString.remove(len);
        jsonString = jsonString + "]";
        return jsonString;
  }
