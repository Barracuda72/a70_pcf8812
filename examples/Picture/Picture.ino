#include <PCF8812_new.h> 
#include "girl.h"

void setup(){
  LcdInit(10, 9, 8, 7, 6);  //CS, RESET, D/C, CLK, DATA
}

void loop(){
  LcdPageONE();
  do{
    LcdBitmap(2, 0, girl, ON);
  }
  while(LcdPageTWO());
  delay(1000);
}

