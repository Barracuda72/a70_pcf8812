#include "Arduino.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <util/delay.h>
#include <compat/deprecated.h>
#include <avr/interrupt.h>
//****************************************************************************************
#ifndef PCF8812_NEW_H
#define PCF8812_NEW_H
//----------------------------------------------------------------------------------------
// LCD Siemens A70  101x64 by H`UGO
//
//
//  -----------------------------------
// |                                   |
// |             O O O O O O O O O O   |
// |             | | | | | | | | | |   |
// |             | | | | | | | | | |   |
// |             L L L G V D C D R C   |
// |             e e e n c a l / e s   |
// |             d d d d c t k C s     |
// |             1   2           e     |
// |             + - +           t     |
// |                                   |
// |                                   |
// |                                   |
// |                                   |
//  -----------------------------------
//

//******************************************************************************

#define LCD_C LOW  // Команда
#define LCD_D HIGH // Данные
//******************************************************************************
#define X_RES	101		           // разрешение по горизонтали 102 -1      97 для Nokia 3410
#define Y_RES	64      	       // разрешение по вертикали 65 - 1
static uint8_t Lcd_page[X_RES][3]; // объявляем массив размером 3 банка

                                 // Макросы для работы с битами
#define ClearPix(reg, bit)       reg &= (~(1<<(bit)))   //reg = reg & (~(1<<bit))   // reg = reg & (~bit)
#define SetPix(reg, bit)         reg |= (1<<(bit))	    //reg = reg | (1<<(bit))    // reg = reg | bit       //reg |= _BV(bit);
#define InvPix(reg, bit)         reg ^= 1<<bit	        //reg = reg ^ (1<<bit)      // reg = reg ^ bit

#define ON	0  //включение 
#define OFF	1  //выключение
#define INV 2  //инверсия

//******************************************************************************
// Прототипы функций************************************************************
void LcdInit(uint8_t _SCE, uint8_t _RESET, uint8_t _SDIN, uint8_t _SCLK, uint8_t _SDAT);
void LcdWrite(uint8_t dc, uint8_t data);
void LcdInverse(uint8_t mode);   
uint8_t LcdPageONE(void);
uint8_t LcdPageTWO(void);
void LcdsetFont(uint8_t *pointer);  //выбор шрифта
uint8_t LcdGetChar(uint8_t ch);
void LcdChar(int8_t x, int8_t y, unsigned char ch, uint8_t color, uint8_t size);                       
void LcdPrint(char *characters, uint8_t color, uint8_t size);                     
void LcdPrintW(char *characters, uint8_t color);  
void LcdPrint(int16_t i, uint8_t color, uint8_t size);
void LcdPrintW(int16_t i, uint8_t color);
void LcdPrint(uint16_t i, uint8_t color, uint8_t size);
void LcdPrintW(uint16_t i, uint8_t color);   
void LcdPrint(int32_t i, uint8_t color, uint8_t size);   
void LcdPrint(uint32_t i, uint8_t color, uint8_t size); 
void LcdPrint(uint32_t i, uint8_t color, uint8_t size, uint8_t base);
void LcdGotoXY (uint8_t x, uint8_t y);
void LcdPixel(int8_t x, int8_t y, uint8_t color);
void LcdLine(int8_t x1, int8_t y1, int8_t x2, int8_t y2, uint8_t color); 
void LcdVLine(int8_t x, int8_t y, uint8_t v, uint8_t color);               
void LcdHLine(int8_t x, int8_t y, uint8_t h, uint8_t color); 
void LcdRect (int8_t x, int8_t y, uint8_t l, uint8_t h, uint8_t color);     
void LcdFillRect(int8_t x, int8_t y, int8_t l, int8_t h, uint8_t color); 
void LcdCircle(int8_t x, int8_t y, uint8_t r, uint8_t color);              
void LcdDisk  (int8_t x, int8_t y, uint8_t radius, uint8_t color);          
void LcdCircleHelper( int8_t x, int8_t y, int8_t r, int8_t cornername, uint8_t color);  
void LcdFillCircleHelper(int8_t x, int8_t y, int8_t r, int8_t cornername, int8_t delta, uint8_t color);
void LcdRoundRect(int8_t x, int8_t y, int8_t w, int8_t h, int8_t r, uint8_t color);  
void LcdFillRoundRect(int8_t x, int8_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color);     
void LcdBitmap(int8_t x, int8_t y, const uint8_t *picture, uint8_t color);
//******************************************************************************
//void LcdTest(uint8_t bank, uint8_t color);
#endif

