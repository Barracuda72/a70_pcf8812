#include <PCF8812_new.h> 
#include "font_5x5.h"
#include "font_5x8.h"
#include "font_7x15.h"
#include "font_10x16.h"

void setup(){
  LcdInit(10, 9, 8, 7, 6);  //CS, RESET, D/C, CLK, DATA
}

void loop(){
  LcdPageONE();
  do{
    LcdsetFont(font_5x5);
    LcdGotoXY(0, 0);
    LcdPrint("1234567890abcABCD\n", ON, 1);
    LcdsetFont(font_5x8);
    LcdPrint("1234567890abcdабв\n", ON, 1);
    LcdsetFont(font_7x15);
    LcdPrint("1234567890\n", ON, 1);
    LcdsetFont(font_10x16);
    LcdPrint("012345678", ON, 1);
  }
  while(LcdPageTWO());
  delay(1000);
}
