#include <PCF8812_new.h> 
#include "e_e.h"
byte a, b;
boolean i, j;

void setup(){
  LcdInit(10, 9, 8, 7, 6);  //CS, RESET, D/C, CLK, DATA
}

void loop(){
  LcdPageONE();
  do{
    LcdFillRect(40, 10, 10, 44, ON);
    LcdBitmap(a, b, e_e, INV);
    LcdFillRect(60, 10, 8, 44, OFF);
  }
  while(LcdPageTWO());

  if(i) a++;
  else a--;
  if(a > 50) i = !i;
  if(j) b++;
  else b--;
  if(b > 17) j = !j;
  delay(100);
}


