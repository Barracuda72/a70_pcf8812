#include <PCF8812_new.h> 
byte a, b;
boolean i, j;

void setup(){
  LcdInit(10, 9, 8, 7, 6);  //CS, RESET, D/C, CLK, DATA
}

void loop(){
  LcdPageONE();
  do{
    LcdFillRoundRect(5, 5, 85, 20, 4, ON);
    LcdDisk(15, 40, 7, ON);
    LcdCircle(a, b, 5, INV);
    LcdVLine(40, 25, 37, INV);
    LcdHLine(5, 41, 36, INV);
    LcdLine( 10, 50, 65, 10, INV);
    LcdFillRect(50, 35, 20, 25, ON);
    LcdRect(73, 35, 20, 25, ON);
  }
  while(LcdPageTWO());
  delay(100);
    if(i) a++;
  else a--;
  if(a > 100) i = !i;
  if(j) b++;
  else b--;
  if(b > 54) j = !j;
}

