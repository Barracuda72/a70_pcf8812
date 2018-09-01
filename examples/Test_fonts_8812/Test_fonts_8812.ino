#include <PCF8812_new.h> 
#include "font_16x32.h"
#include "font_5x8.h"
#include "font_16x24.h"
#include "font_10x16.h"
uint8_t var, st, en;

void setup(){
  LcdInit(2, 1, 0, 12, 13);
  LcdsetFont(font_16x24);  // тестовый
  var = LcdGetChar(1);
  st = var;
  en = LcdGetChar(2);
}

void loop(){
  LcdPageONE();
  LcdGotoXY(0, 0);
  do{
    LcdGotoXY(0, 0);
    LcdsetFont(font_16x32);
    LcdPrint(var, ON, 1);
    LcdGotoXY(60, 0);
    LcdsetFont(font_16x24);  // тестовый 
    LcdChar(60, 0, var, ON, 1);
    LcdGotoXY(60, 32);
    LcdsetFont(font_5x8);
    LcdChar(60, 32, var, ON, 1);
  }
  while(LcdPageTWO());
  var++;
  if(var > en) var = st;
  delay(800);
}

