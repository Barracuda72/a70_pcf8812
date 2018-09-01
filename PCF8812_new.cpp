#include <inttypes.h>
#if defined(__AVR__)
# include <util/delay.h>
#include <wiring.h>
#endif
#if ARDUINO >= 100
# include "Arduino.h"
#else
# include "WProgram.h"
#endif

#include "PCF8812_new.h"
//*******************************************************************************************************
uint8_t *FontPointer;    //��������� ������
uint8_t FONT_WIDTH, FONT_HEIGHT, FONT_START, FONT_END, qbit, type, idfont;     //��������� ������
int8_t x_curr, y_curr, widht, PIN_SCE, PIN_SDIN, PIN_RESET, PIN_SCLK, PIN_SDAT; 
uint8_t page;

//*******************************************************************************************************
//������������� ������ **********************************************************************************
void LcdInit(uint8_t _SCE, uint8_t _RESET, uint8_t _SDIN, uint8_t _SCLK, uint8_t _SDAT)  //CS, RESET, D/C, CLK, DATA
{
	PIN_SCE  = _SCE  ;
	PIN_SDIN = _SDIN ;
	PIN_RESET= _RESET;
	PIN_SCLK = _SCLK ;
	PIN_SDAT = _SDAT ;  
	pinMode(PIN_SCE,  OUTPUT);
    pinMode(PIN_RESET,OUTPUT);
    pinMode(PIN_SDIN, OUTPUT);
    pinMode(PIN_SCLK, OUTPUT);
	pinMode(PIN_SDAT, OUTPUT);
	
	_delay_us(10);                // LCD RESET
    digitalWrite(PIN_SDAT, LOW);  //
    digitalWrite(PIN_SCLK, LOW);  //
    digitalWrite(PIN_SDIN, LOW);  //
    _delay_us(10);                //
    digitalWrite(PIN_RESET, HIGH);//
    _delay_us(10);                //
    digitalWrite(PIN_SCE, LOW);   //
    _delay_us(10);                //
    digitalWrite(PIN_RESET, LOW); //
    _delay_us(10);                //
    digitalWrite(PIN_RESET, HIGH);//
    digitalWrite(PIN_SCE, HIGH);  // END RESET
  delay(75);
	LcdWrite(LCD_C, 0x21);  // Function set: extended instruction set    --21
    LcdWrite(LCD_C, 0x14);  // Bias System                               --14
	LcdWrite(LCD_C, 0x0A);  // HV-gen stages                             --0A
	LcdWrite(LCD_C, 0x05);  // Temperature Control                       --05
	LcdWrite(LCD_C, 0xCC);  // Contrast: 204                             --CC
	LcdWrite(LCD_C, 0x20);  // Function set: standard instruction set    --20
	LcdWrite(LCD_C, 0x11);  // VLCD programming range: high              --11
	LcdWrite(LCD_C, 0x0C);  // Display control: normal (inverted = 0x0D) --0C
	
  LcdWrite(LCD_C, 0x40);    // ��������� ������ �� Y:
  LcdWrite(LCD_C, 0x80);    // ��������� ������ �� X:
  for (int i=0; i<((X_RES+1)*8); i++)    // Lcd Clear
  {                                      //
	LcdWrite(LCD_D, 0x0);                //
  }                                      // end clear
}

//********************************************************************************************
//******* �������� ����� (������� ��� ������) �� LCD-���������� ******************************
//  mode: LCD_C - �������� �������
//		  LCD_D - �������� ������
void LcdWrite(uint8_t dc, uint8_t data)
{
	digitalWrite(PIN_SDAT, LOW);
	digitalWrite(PIN_SCE, LOW);
	digitalWrite(PIN_SCLK, LOW);
	digitalWrite(PIN_SDIN, dc);  // DATA high / COMAND low
    if(dc) shiftOut(PIN_SDAT, PIN_SCLK, LSBFIRST, data);
	else   shiftOut(PIN_SDAT, PIN_SCLK, MSBFIRST, data); 
	digitalWrite(PIN_SCE, HIGH);
}

//*******************************************************************************************
//***************** ������������� ����� �������� ������ *************************************
//  mode: OFF -> ����������, ��� ON -> ���������
void LcdInverse(uint8_t mode)
{
	if (mode) LcdWrite(LCD_C, 0x0C);   //����
	     else LcdWrite(LCD_C, 0x0D);   //���
}

//*******************************************************************************************
//************************ ������� ������ ������ ********************************************
uint8_t LcdPageONE(void)
{
	page = 3;
	return page;
}

//*******************************************************************************************
//*********************** ������� ���������� ������ *****************************************
uint8_t LcdPageTWO(void)
{
	uint8_t step, p, b, m = 3;
	switch(page){
		case 3: step = 0; break;       // 5  ��� 
		case 2: step = 3; break;       // 2  Nokia
		case 1: step = 6; m = 2; break;// 0  3410
	}		              
    LcdWrite(LCD_C, 0x40 + step );    // ��������� ������ �� Y:   � ���� 0...3...6  
    LcdWrite(LCD_C, 0x80);            // ��������� ������ �� X: 
	 for(b = 0; b < m; b++){          //                                  (b = m; b > 0; b--)   ____________________��������� ��� Nikia 3410
		 for(p = X_RES-1; p < X_RES; p--){
			 LcdWrite(LCD_D, Lcd_page[p][b]);  //������� ������������� ������           LcdWrite(LCD_D, Lcd_page[p][b-1]);    ��� Nokia 3410
			 Lcd_page[p][b] = 0b00000000;      //�������� ������
		 }
		 LcdWrite(LCD_D, 0x0);                 //������� 102 � ����������� 
	 }  	
/*	 for(b = 0; b < m; b++){
	     for(p = 0; p < X_RES; p++){
		    Lcd_page[p][b] = 0b00000000;        //�������� ������
	    }		
*/	}
	page--;
	return page;
}
	
//*******************************************************************************************
/*void LcdTest(uint8_t bank, uint8_t color){
	if(bank > 7) bank = 7;
	LcdWrite(LCD_C, 0x40 + bank );    // ��������� ������ �� Y:   
	LcdWrite(LCD_C, 0x80);            // ��������� ������ �� X:
	for(uint8_t p = X_RES-1; p < X_RES; p--){
	if(color)
	  LcdWrite(LCD_D, 0xFF);
	  else
	  LcdWrite(LCD_D, 0x0);
	}	  
}*/
//*********** ��������� � ���������� � ���������� �:0..101 �:0..63 ***************************
void LcdGotoXY (uint8_t x, uint8_t y)
{
	x_curr=x;
	y_curr=y;        
}

//*************** ����� �������� ***********************************************************
//*************** ����� ������� � ���������� x = 0...100  y = 0...63 *********************** 
void LcdPixel(int8_t x, int8_t y, uint8_t color)
{
	if(x < 0 || x >= X_RES || y < 0 || y >= Y_RES) return;         
	uint8_t data;
	     if(y < 24 && page == 3)                     data = Lcd_page[x][y/8];   // ������
	else if((y >= 24 && y < 48) && page == 2){y-=24; data = Lcd_page[x][y/8];}  // ���� �� 
	else if(y >= 48 && page == 1)            {y-=48; data = Lcd_page[x][y/8];}	// �������
		else return;	
	   switch(color){
		   case ON:    SetPix(data, y%8); break;     //data |= _BV(y%8); 
		   case OFF: ClearPix(data, y%8); break;     //data &= ~_BV(y%8);
		   case INV:   InvPix(data, y%8); break;     //data ^= 1<<y%8;
	   }
	   Lcd_page[x][y/8] = data;                      //������� ���� � ������                
}

//******************************************************************************************
//******* �����  ����� �� � = 0...100 y = 0...63 *******************************************
void LcdLine (int8_t x1, int8_t y1, int8_t x2, int8_t y2, uint8_t color)
{
int8_t steep = abs(y2 - y1) > abs(x2 - x1);
  if (steep) {
	  int8_t xy = x1;
	  x1 = y1;  y1 = xy;
	  xy = x2;  x2 = y2;  y2 = xy;
  }
  if (x1 > x2) {
	  int8_t xy = x1;
	  x1 = x2;  x2 = xy;
	  xy = y1;  y1 = y2;  y2 = xy;
  }
  int8_t difx, dify;
  difx = x2 - x1;
  dify = abs(y2 - y1);
  int8_t err = difx / 2;
  int8_t ystep;
  if (y1 < y2) {
    ystep = 1;
  } else {
    ystep = -1;
  }
  for (; x1 <= x2; x1++) {
    if (steep) {
      LcdPixel(y1, x1, color);
    } else {
      LcdPixel(x1, y1, color);
    }
    err -= dify;
    if (err < 0) {
      y1 += ystep;
      err += difx;
    }
  }	
}

//******************************************************************************************
//******* ����� ������������ ����� �������� v = 1...64 *************************************
void LcdVLine (int8_t x, int8_t y, uint8_t v, uint8_t color)
{
	for(byte i = 0; i < v; i++){
		LcdPixel(x, y, color);
		y++;
		 if(y >= Y_RES) break;
	}
}
		 
//******************************************************************************************
//********** ����� �������������� ����� ������� h = 1..101 *********************************
void LcdHLine (int8_t x, int8_t y, uint8_t h, uint8_t color)
{
	for(byte b = 0; b < h; b++){
		LcdPixel(x, y, color);
		x++;
		 if(x >= X_RES) break;
	}
}

//******************************************************************************************
//*********** ����� �������������� ������� 1..101, ������� 1..64 ***************************
void LcdRect(int8_t x, int8_t y, uint8_t l, uint8_t h, uint8_t color)
{
	for(byte i = 0; i < l; i++){
		if((x+i) > X_RES) break;
		for(byte j = 0; j < h; j++){
			if((i > 0 && i < (l-1)) && j == 1) j = (h-1);
			if((y+j) >Y_RES) break;
			LcdPixel(x+i, y+j, color);
		}
	}
}

//******************************************************************************************
//*********** ����� ������������� �������������� ������� 1..101, ������� 1..64 *************
void LcdFillRect(int8_t x, int8_t y, int8_t l, int8_t h, uint8_t color)
{
	for(byte i = 0; i < l; i++){
		if((x+i) > X_RES) break;
		for(byte j = 0; j < h; j++){
			if((y+j) >Y_RES) break;
			LcdPixel(x+i, y+j, color);
		}
	}	
}

//*******************************************************************************************
//************************** ����� ���������� ***********************************************
void LcdCircle(int8_t x, int8_t y, uint8_t r, uint8_t color) 
{
  int8_t err, x1, y1;
  err = -r;
  x1   = r;
  y1   = 0;
     LcdPixel(x, y + r, color);
     LcdPixel(x, y - r, color);
     LcdPixel(x + r, y, color);
     LcdPixel(x - r, y, color);
  while(x1 >= y1)
  {
    LcdPixel(x + x1, y + y1, color);
    LcdPixel(x - x1, y + y1, color);
    LcdPixel(x + x1, y - y1, color);
    LcdPixel(x - x1, y - y1, color);
    LcdPixel(x + y1, y + x1, color);
    LcdPixel(x - y1, y + x1, color);
    LcdPixel(x + y1, y - x1, color);
    LcdPixel(x - y1, y - x1, color);

    err += y1;
    y1++;
    err += y1;
    if(err >= 0)
    {
      x1--;
      err -= x1;
      err -= x1;
    }
  }
}

//*******************************************************************************************
//************************** ����� ���������� ���������� ************************************
void LcdDisk(int8_t x, int8_t y, uint8_t r, uint8_t color)
{
	if(color == INV) color = ON; // �������� �� ���������
   LcdVLine(x, y - r, 2*r + 1, color);
   LcdFillCircleHelper(x, y, r, 3, 0, color);
}

//*******************************************************************************************
//***************************** �������� ���������� *****************************************
void LcdCircleHelper( int8_t x, int8_t y, int8_t r, int8_t cornername, uint8_t color) 
{
  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x1 = 0;
  int8_t y1 = r;
  while (x1 < y1) {
    if (f >= 0) {
      y1--;
      ddF_y += 2;
      f += ddF_y;
    }
    if (cornername & 0x4) {
      LcdPixel(x + x1, y + y1, color);
      LcdPixel(x + y1, y + x1, color);
    }
    if (cornername & 0x2) {
      LcdPixel(x + x1, y - y1, color);
      LcdPixel(x + y1, y - x1, color);
    }
    if (cornername & 0x8) {
      LcdPixel(x - y1, y + x1, color);
      LcdPixel(x - x1, y + y1, color);
    }
    if (cornername & 0x1) {
      LcdPixel(x - y1, y - x1, color);
      LcdPixel(x - x1, y - y1, color);
    }
	x1++;
    ddF_x += 2;
    f += ddF_x;
  }
}

//*******************************************************************************************
//***************************** ������� �������� (��������) *********************************
void LcdFillCircleHelper(int8_t x, int8_t y, int8_t r, int8_t cornername, int8_t delta, uint8_t color) 
{
	if(color == INV) color = ON;
  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x1 = 0;
  int8_t y1 = r;
  while (x1 < y1) {
    if (f >= 0) {
      y1--;
      ddF_y += 2;
      f += ddF_y;
    }
    x1++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername & 0x1) {
      LcdVLine(x + x1, y - y1, 2*y1 + 1 + delta, color);
      LcdVLine(x + y1, y - x1, 2*x1 + 1 + delta, color);
    }
    if (cornername & 0x2) {
      LcdVLine(x - x1, y - y1, 2*y1 + 1 + delta, color);
      LcdVLine(x - y1, y - x1, 2*x1 + 1 + delta, color);
    }
  }
}

//*****************************************************************************************
//********************* ���������� ������������� *****************************************
void LcdRoundRect(int8_t x, int8_t y, int8_t w, int8_t h, int8_t r, uint8_t color) 
{
	if(color == INV) color = ON;
   LcdHLine(x + r, y, w - 2*r, color);          // ����
   LcdHLine(x + r, y + h - 1, w - 2*r, color);  // ���
   LcdVLine(x, y + r, h - 2*r, color);          // ����
   LcdVLine( x + w - 1, y + r, h - 2*r, color); // �����
  LcdCircleHelper(x + r, y + r, r, 1, color);
  LcdCircleHelper(x + w - r - 1, y + r, r, 2, color);
  LcdCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
  LcdCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

//*****************************************************************************************
//********************* ���������� ���������� ������������� ******************************
void LcdFillRoundRect(int8_t x, int8_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color) 
{
	if(color == INV) color = ON;
  LcdFillRect(x + r, y, w - 2*r, h, color);
  LcdFillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2*r - 1, color);
  LcdFillCircleHelper(x + r , y + r, r, 2, h - 2*r - 1, color);
}

//*******************************************************************************************
//********** ����� ����������� ����������� �� ����������� ������ ****************************
 void LcdBitmap(int8_t x, int8_t y, const uint8_t *picture, uint8_t color) 
 {
	   uint8_t pwidth  = pgm_read_byte(&picture[0]);            // ������ �������� � ��������  
       uint8_t pheight = pgm_read_byte(&picture[1]);            // ������ �������� � ��������
  for (byte j = 0; j < pheight; j++) {
    for (byte i = 0; i < pwidth; i++ ) {
      if (pgm_read_byte(picture + 2 + i + (j/8)*pwidth) & _BV(j%8)) {
  LcdPixel(x + i, y + j, color);
      }
    }
  }
}

//************** ������ �� ������� **********************************************************
//************** ����� ������ � ��������� ��� ���������� ************************************
void LcdsetFont(uint8_t *pointer)
{
 FontPointer = pointer;
  qbit       = pgm_read_byte_near(&FontPointer[0]); // ���-�� ���-������������
  type       = pgm_read_byte_near(&FontPointer[1]); // ����������� (������ � ������ - 8 || 16 || 24 || 32...)
  FONT_WIDTH = pgm_read_byte_near(&FontPointer[2]); // ���������� ������
  FONT_HEIGHT= pgm_read_byte_near(&FontPointer[3]); // ���������� ������
  idfont     = pgm_read_byte_near(&FontPointer[4]); // ������ ������ ��� �������� �������� (!.,: ) 1 ��� 2 - ������ ������
  FONT_START = pgm_read_byte_near(&FontPointer[5]); // ���������� ������ ������ � �������
  FONT_END   = pgm_read_byte_near(&FontPointer[6]); // ��������� ������ � �������
}
 //******************************************************************************************
 //************ ����� ������� ������� �������� (��� ��������� ����������� ������)************
uint8_t LcdGetChar(uint8_t ch)
{
if(ch == 1) return FONT_START;
if(ch == 2) return FONT_END;
}

//*******************************************************************************************
//************ ����� ������� � ������� ����� ************************************************
void LcdChar(int8_t x, int8_t y, unsigned char ch, uint8_t color, uint8_t size)
{ 
	if(ch > FONT_END) return;
	if(ch == '\n' || (FONT_WIDTH * size + x_curr) > X_RES) {           //����� �������� �� �������
     y_curr += (FONT_HEIGHT * size + 1); //������� �� ����� ������ (1 ������� ����� �������� (+1))
	 x_curr = 0;
	 if(ch == '\n') return;              //��� ������ �������� �� �������� 
	  }	
	  if(ch == '\t') {x_curr +=  FONT_WIDTH; return;}
	x = x_curr; y = y_curr;
	for(byte j = 0; j < FONT_HEIGHT; j++){
	  for (byte index = 0; index < FONT_WIDTH + (FONT_WIDTH * widht); index++){
    if((pgm_read_byte(&FontPointer[(ch - FONT_START)* (FONT_WIDTH * (type/8)) + qbit + index / (1+widht) + FONT_WIDTH*(j/8)])) & _BV(j%8)){
		if(size == 1){   //���������� ������
          LcdPixel(x + index, y + j, color);
		    if(widht){   //������� ������
			  index++;
			    LcdPixel(x + index, y + j, color);
		      }
		   }
		   else{         //������� ������
			   LcdFillRect(x + (index * size), y + (j * size), size, size, color);
		   }			   			  			
	    }
		if((ch == 33||ch == 44||ch == 46||ch == 58) && index == 4 + (index*widht) && idfont==1)  break;             //.,:!	����������� �������   
	  }		
    }
	 x_curr += ((FONT_WIDTH * widht + FONT_WIDTH+1) * size);      // ����� ��������� 1 ������� (+1)
	if((ch == 33||ch == 44||ch == 46||ch == 58) && idfont==1) x_curr -= ((FONT_WIDTH - 4) * size);
}

//*******************************************************************************************
//******* ����� ������ �������� � ������� ����� � = 0...100  y = 0...63 *********************
void LcdPrint(char *characters, uint8_t color, uint8_t size)
{
    int8_t x = x_curr;
	int8_t y = y_curr;
  while (*characters){
    LcdChar(x, y, *characters++, color, size);
  }
}

//******************************************************************************
// ����� ������ ������� �������� � ������� ����� *******************************
void LcdPrintW(char *characters, uint8_t color)
{
	widht++;
    int8_t x = x_curr;
	int8_t y = y_curr;
  while (*characters){
    LcdChar(x, y, *characters++, color, 1);
  }
  widht = 0;
}

//******************************************************************************
// ����� �������� ���������� �������� 1 � 2 ����� � ������� ����� **************
void LcdPrint(int16_t i, uint8_t color, uint8_t size)
{
	char output[] = "xxxxxx";
	uint8_t num = 5;
	boolean sig = 0;
	if(i < 0) {
		sig = 1;
		i = -i;
	}
	do{
		output[num] = ((i%10) + 48);
		i /= 10;
		num--;
	}
	while(i);
	if(sig) {
		output[num] = 45;   // ���� "-"
	}
	char *f_ptr = output;
	int8_t x = x_curr;
	int8_t y = y_curr;
	while(*f_ptr){
		if(*f_ptr == 'x') {
			*f_ptr++;
			continue;
		}
		LcdChar(x, y, *f_ptr++, color, size);
	}
}

//******************************************************************************
// ����� ������� �������� ���������� � ������� ����� ***************************
void LcdPrintW(int16_t i, uint8_t color)
{ 
  widht++;
  LcdPrint(i, color, 1);
  widht = 0;
}

//******************************************************************************
// ����� ����������� ���������� �������� 1 � 2 ����� � ������� ����� ***********
void LcdPrint(uint16_t i, uint8_t color, uint8_t size)
{
	char output[] = "xxxxx";
	uint8_t num = 4;
	do{
		output[num] = ((i%10) + 48);
		i /= 10;
		num--;
	}
	while(i);
	char *f_ptr = output;
	int8_t x = x_curr;
	int8_t y = y_curr;
	while(*f_ptr){
		if(*f_ptr == 'x') {
			*f_ptr++;
			continue;
		}
		LcdChar(x, y, *f_ptr++, color, size);
	}
}

//******************************************************************************
// ����� ������� ����������� ���������� � ������� ����� ************************
void LcdPrintW(uint16_t i, uint8_t color)
{
	widht++;
	LcdPrint(i, color, 1);
	widht = 0;
}

//******************************************************************************
//����� ������������� �������� ���������� *************************************
void LcdPrint(int32_t i, uint8_t color, uint8_t size)
{
	char output[] = "xxxxxxxxxxx";
	uint8_t num = 10;
	boolean sig = 0;
	if(i < 0) {
		sig = 1;
		i = -i; //abs(i);
	}
	do{
		output[num] = ((i%10) + 48);
		i /= 10;
		num--;
	}
	while(i);
	if(sig) {
		output[num] = 45;   // ���� "-"
	}
	char *f_ptr = output;
	int8_t x = x_curr;
	int8_t y = y_curr;
	while(*f_ptr){
		if(*f_ptr == 'x') {
			*f_ptr++;
			continue;
		}
		LcdChar(x, y, *f_ptr++, color, size);
	}
}

//******************************************************************************
//����� ������������� ����������� ���������� **********************************
void LcdPrint(uint32_t i, uint8_t color, uint8_t size)
{
	char output[] = "xxxxxxxxxx";
	uint8_t num = 9;
	do{
		output[num] = ((i%10) + 48);
		i /= 10;
		num--;
	}
	while(i);
	char *f_ptr = output;
	int8_t x = x_curr;
	int8_t y = y_curr;
	while(*f_ptr){
		if(*f_ptr == 'x') {
			*f_ptr++;
			continue;
		}
		LcdChar(x, y, *f_ptr++, color, size);
	}
}

//******************************************************************************
// ����� � �������� HEX, BIN, OCT, DEC *****************************************
void LcdPrint(uint32_t i, uint8_t color, uint8_t size, uint8_t base)
{
	int16_t a, b;
	a = i>>16;
	b = i;
	if(base == HEX || base == OCT || base == BIN)
	{
	  if(a){
		char x[10];
		itoa(a, x, base);
		LcdPrint(x, color, size);
	  }
	  if(b){
		char y[10];
		itoa(b, y, base);
		LcdPrint(y, color, size);
	  }
	}
	else{
		LcdPrint(i, color, size);
	}	
}

//*******************************************************************************************