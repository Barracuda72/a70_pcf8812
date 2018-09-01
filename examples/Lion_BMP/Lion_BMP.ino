#include <PCF8812_new.h> 
#include "lion.h"
#include "font_5x8.h"
#include "font_5x5.h"
void setup(){
  LcdInit(10, 9, 8, 7, 6);  //CS, RESET, D/C, CLK, DATA
  LcdsetFont(font_5x8);
}
void loop(){

  LcdPageONE();
  do{
    LcdRoundRect(0, 0, 101, 64, 3, ON);
    LcdsetFont(font_5x8);
    LcdGotoXY(8, 2);
    LcdPrintW("HELLO", ON);
    LcdGotoXY(42, 11);
    LcdPrintW("WORLD", ON);
    LcdBitmap(2, 11, lion, ON);
    LcdsetFont(font_5x5);
    LcdGotoXY(52, 17);
    LcdPrint("by H`UGO", ON, 1);
  }
  while(LcdPageTWO());

  delay(1000);
}


