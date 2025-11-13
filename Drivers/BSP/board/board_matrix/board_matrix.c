
/* Includes ------------------------------------------------------------------*/
#include "board_matrix.h"
#include "matrix_ll.h"
#include "regConf_FM6353.h"
#include "main.h"
#include "board_led.h"


#define MATRIX_TIMER            30 //ms
#define MATRIX_REINIT_TIMER     10000 //ms
#define MIN_BRIGHT              0x210
#define MAX_BRIGHT              0x3FF



uint32_t test_count_rander = 0;

static uint8_t screenBuffer[MATRIX_BUFF_SIZE];

static uint32_t matrixTime;
static uint32_t matrixReInitTime;
static volatile uint8_t updateMatrix = 0;

static uint16_t newBright = 100;
static uint16_t currentBright = 100;

static uint16_t _x_start, _y_start, _x_end, _y_end;
static uint16_t _color565;
static uint16_t *_buff;
static volatile uint8_t newTask = 0;

static uint16_t red16Bit;
static uint16_t green16Bit;
static uint16_t blue16Bit;
static uint16_t bufferPos;
static uint16_t inputMask;
static uint16_t x_pos,y_pos;
static uint16_t color565;
static uint16_t *pBuff;
static uint8_t outputmask;
static uint8_t rgb;
static uint8_t x_byte;


static const uint16_t LUT_5[32] = {
     0,  68, 273, 614,1091,1705,2455,3342,4364,5524,6819,8252,9820,11525,13366,15344,
  17458,19708,22095,24618,27278,30074,33006,36075,39280,42622,46100,49714,53465,57352,61375,65535,
};
static const uint16_t LUT_6[64] = {
     0,  17,  66, 149, 264, 413, 594, 809,1057,1337,1651,1998,2378,2790,3236,3715,
  4227,4772,5350,5961,6605,7282,7992,8735,9511,10320,11162,12037,12945,13886,14861,15868,
  16908,17981,19088,20227,21399,22605,23843,25114,26419,27756,29127,30530,31967,33436,34939,36474,
  38043,39645,41279,42947,44648,46381,48148,49948,51781,53647,55545,57477,59442,61440,63471,65535,
  };


static void matrixRefresh(void);  
static void matrixSetup(void);
static void matrixReInit(void);
static void matrixBright(void);
static void matrixTest(void);

void Matrix_Periodic_Handler(){
uint32_t localtime = SystemTimer_GetTick();
 
  if(newTask == 1){
    MatrixDrawRect(_x_start,_y_start,_x_end,_y_end,_color565);
    newTask = 0;
    return;
  }
  if(newTask == 2){
    MatrixRenderRect(_x_start,_y_start,_x_end,_y_end,_buff);
    newTask = 0;
    return;
  }

  if(newTask == 3){
    updateMatrix = 1;
    newTask = 0;
    return;
  }


  if(localtime - matrixTime > MATRIX_TIMER){
    updateMatrix += 1;
    matrixRefresh();
    matrixTime = localtime;
  }
  
  if(localtime - matrixReInitTime > MATRIX_REINIT_TIMER){
    matrixReInit();
    matrixReInitTime = localtime;
  }
  
  matrixBright();
}


void inline MatrixDrawPixel(uint16_t x, uint16_t y, uint16_t color565){
  uint16_t red16Bit = LUT_5[((color565&0xf800) >> 11)];
  uint16_t green16Bit = LUT_6[((color565&0x07e0) >> 5)];
  uint16_t blue16Bit = LUT_5[(color565&0x001f )];
 

  if(y > DISPLAY_SCAN_LINES - 1){
    bufferPos = x * BYTE_FOR_PIXEL + MATRIX_BUFF_LINE_SIZE *(y - DISPLAY_SCAN_LINES);

    rgb = 0x00;
    if(red16Bit & 0x8000) rgb += 8;
    if(green16Bit & 0x8000) rgb += 0x10;
    if(blue16Bit & 0x8000) rgb += 0x20;
    screenBuffer[bufferPos] = (screenBuffer[bufferPos] & 0xC7) | rgb;
    
    rgb = 0x00;
    if(red16Bit & 0x4000) rgb += 8;
    if(green16Bit & 0x4000) rgb += 0x10;
    if(blue16Bit & 0x4000) rgb += 0x20;
    screenBuffer[bufferPos + 1] = (screenBuffer[bufferPos + 1] & 0xC7) | rgb;
    
    rgb = 0x00;
    if(red16Bit & 0x2000) rgb += 8;
    if(green16Bit & 0x2000) rgb += 0x10;
    if(blue16Bit & 0x2000) rgb += 0x20;
    screenBuffer[bufferPos + 2] = (screenBuffer[bufferPos + 2] & 0xC7) | rgb;
    
    rgb = 0x00;
    if(red16Bit & 0x1000) rgb += 8;
    if(green16Bit & 0x1000) rgb += 0x10;
    if(blue16Bit & 0x1000) rgb += 0x20;
    screenBuffer[bufferPos + 3] = (screenBuffer[bufferPos + 3] & 0xC7) | rgb;
  }
  else{
    bufferPos = x * BYTE_FOR_PIXEL + y * MATRIX_BUFF_LINE_SIZE;
    
    //rgb = 0x00;
    //if(red16Bit & 0x8000) rgb += 1;
    rgb = red16Bit >> 15;
    if(green16Bit & 0x8000) rgb += 2;
    if(blue16Bit & 0x8000) rgb += 4;
    screenBuffer[bufferPos] = (screenBuffer[bufferPos] & 0xf8) | rgb;
    
    rgb = 0x00;
    if(red16Bit & 0x4000) rgb += 1;
    if(green16Bit & 0x4000) rgb += 2;
    if(blue16Bit & 0x4000) rgb += 4;
    screenBuffer[bufferPos + 1] = (screenBuffer[bufferPos + 1] & 0xf8) | rgb;
    
    rgb = 0x00;
    if(red16Bit & 0x2000) rgb += 1;
    if(green16Bit & 0x2000) rgb += 2;
    if(blue16Bit & 0x2000) rgb += 4;
    screenBuffer[bufferPos + 2] = (screenBuffer[bufferPos + 2] & 0xf8) | rgb;

    rgb = 0x00;
    if(red16Bit & 0x1000) rgb += 1;
    if(green16Bit & 0x1000) rgb += 2;
    if(blue16Bit & 0x1000) rgb += 4;
    screenBuffer[bufferPos + 3] = (screenBuffer[bufferPos + 3] & 0xf8) | rgb;
  }
    
}

void MatrixDrawRect(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color565){
  uint16_t red16Bit = LUT_5[((color565&0xf800) >> 11)];
  uint16_t green16Bit = LUT_6[((color565&0x07e0) >> 5)];
  uint16_t blue16Bit = LUT_5[(color565&0x001f )];
  
  for (y_pos = y_start; y_pos <= y_end; y_pos++){
    for (x_pos = x_start; x_pos <= x_end; x_pos++){
      bufferPos = x_pos * BYTE_FOR_PIXEL + y_pos * MATRIX_BUFF_LINE_SIZE;
      outputmask = 0x07;
      inputMask = 0x8000;
      if(y_pos > DISPLAY_SCAN_LINES - 1){
        outputmask = 0x38;//b00111000
        bufferPos -= DISPLAY_SCAN_LINES * MATRIX_BUFF_LINE_SIZE;
      }
      for(x_byte = 0; x_byte < BYTE_FOR_PIXEL; x_byte++) {
        if( bufferPos + x_byte >= MATRIX_BUFF_SIZE)
          continue; 
        rgb = 0x00;
        inputMask = 0x8000 >> x_byte;
        if(red16Bit & inputMask) rgb += 1;
        if(green16Bit & inputMask) rgb += 2;
        if(blue16Bit & inputMask) rgb += 4;
        if(y_pos > DISPLAY_SCAN_LINES - 1) rgb <<= 3;
        
        screenBuffer[bufferPos + x_byte] = (screenBuffer[bufferPos + x_byte] & ~outputmask) | rgb;
      }
    }
  }
  updateMatrix += 1;
}

void MatrixRenderRect(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t* buff){
  pBuff = buff;
  for (y_pos = y_start; y_pos <= y_end; y_pos++){
    for (x_pos = x_start; x_pos <= x_end; x_pos++){
      color565 = *pBuff++;
      red16Bit = LUT_5[((color565&0xf800) >> 11)];
      green16Bit = LUT_6[((color565&0x07e0) >> 5)];
      blue16Bit = LUT_5[(color565&0x001f )];
      bufferPos = x_pos * BYTE_FOR_PIXEL + y_pos * MATRIX_BUFF_LINE_SIZE;
      outputmask = 0x07;
      inputMask = 0x8000;
      if(y_pos > DISPLAY_SCAN_LINES - 1){
        outputmask = 0x38;//b00111000
        bufferPos -= DISPLAY_SCAN_LINES * MATRIX_BUFF_LINE_SIZE;
      }
      for(x_byte = 0; x_byte < BYTE_FOR_PIXEL; x_byte++) {
        rgb = 0x00;
        inputMask = 0x8000 >> x_byte;
        if(red16Bit & inputMask) rgb += 1;
        if(green16Bit & inputMask) rgb += 2;
        if(blue16Bit & inputMask) rgb += 4;
        if(y_pos > DISPLAY_SCAN_LINES - 1) rgb <<= 3;
        screenBuffer[bufferPos + x_byte] = (screenBuffer[bufferPos + x_byte] & ~outputmask) | rgb;
      }
    }
  }
  updateMatrix += 1;
}

void MatrixSetDrawRect(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color565){
  _x_start = x_start;
  _y_start = y_start;
  _x_end = x_end;
  _y_end = y_end;
  _color565 = color565;
  newTask = 1;
}
void MatrixSetRenderRect(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t* buff){
  _x_start = x_start;
  _y_start = y_start;
  _x_end = x_end;
  _y_end = y_end;
  _buff = buff;
  newTask = 2;
}

void MatrixSetUpdate(void){
  newTask = 3;
}

uint8_t isMatrixBusy(void){
  return newTask; 
}      
void Matrix_Init(void)
{
  currentBright = Modul.brightness;
  MatrixSetBrightness(currentBright);
  matrixInit();
  matrixSetup();
  matrixTest();
  matrixPwmStart();
}

void MatrixSetBrightness(uint8_t bright){
  newBright = bright;
}

static void matrixBright(void){
  if (currentBright == newBright)
    return;
  currentBright = newBright; 
  uint16_t tmp = (MIN_BRIGHT + (MAX_BRIGHT - MIN_BRIGHT)*currentBright/100)& REG_2_GAIN_MASK;
  registers[1].valueR = (registers[1].valueG & ~REG_2_GAIN_MASK) | tmp; 
  registers[1].valueG = (registers[1].valueG & ~REG_2_GAIN_MASK) | tmp; 
  registers[1].valueB = (registers[1].valueG & ~REG_2_GAIN_MASK) | tmp; 
  
  matrixVerticalSync();  
  matrixDelay(1000);
  matrixSendPreAct();
  matrixSendConfReg(registers[1]);
  updateMatrix = 1;  
}

static void matrixTest(void){
  MatrixDrawRect(0,0,255,63,COLOR_WHITE);
  MatrixDrawRect(10,10,255-10,63-10,COLOR_RED);
  MatrixDrawRect(20,20,255-20,63-20,COLOR_GREEN);
  MatrixDrawRect(30,30,255-30,63-30,COLOR_BLUE);
}

static void matrixSetup(void){
  uint32_t i = 0;
  for (i = 0; i < MATRIX_BUFF_SIZE; i++) 
    screenBuffer[i] = 0;
  
  matrixSendPreAct();
  matrixSendOn();
  matrixSendVerticalSync();

  for(i = 0; i < REG_NUM; i++){
    matrixDelay(1000);
    matrixSendPreAct();
    matrixSendConfReg(registers[i]);
  }


//  
//  matrixSendLatch(14); // Pre-active command
//  matrixSendLatch(12); // Enable all output channels
//  matrixSendLatch(3); // Vertical sync swap frame buffs
  
  
  
//  matrixDelay(1000);
//  matrixSendLatch(14); // Pre-active command
//  matrixSendConfiguration(0x1f00, 4);// количкство сканируемых линий 
//  
//  
//  matrixDelay(1000);
//  matrixSendLatch(14); // Pre-active command
//  matrixSendConfiguration(getBright(), 6); //яркость
//  

//  matrixDelay(1000);
//  matrixSendLatch(14); // Pre-active command
//  matrixSendConfiguration(0x0000, 8);//b3b2b1b0 - чумто управл€ет  
//  
//  matrixDelay(1000);
//  matrixSendLatch(14); // Pre-active command
//  matrixSendConfiguration(0x0000, 10);// b6 = 1 - какой тщ тест  
  
}

static void matrixReInit(void){
  uint32_t i = 0;

  matrixVerticalSync();
  matrixSendPreAct();
  matrixSendOn();

  for(i = 0; i < REG_NUM; i++){
    matrixDelay(1000);
    matrixSendPreAct();
    matrixSendConfReg(registers[i]);
  }
  updateMatrix = 1;
  
//  matrixSendLatch(14); // Pre-active command
//  matrixSendLatch(12); // Enable all output channels
//  //sendLatch(13); // Disable all output channels
//  //matrixSendLatch(3); // Vertical sync swap frame buffs
//  
//  matrixDelay(1000);
//  matrixSendLatch(14); // Pre-active command
//  matrixSendConfiguration(0x1f00, 4);// количкство сканируемых линий 
//  
//  
//  matrixDelay(1000);
//  matrixSendLatch(14); // Pre-active command
////  matrixSendConfiguration(0x01ff, 6);
//  matrixSendConfiguration(getBright(), 6); //яркость

//  matrixDelay(1000);
//  matrixSendLatch(14); // Pre-active command
//  matrixSendConfiguration(0x0000, 8);//b3b2b1b0
//  
//  matrixDelay(1000);
//  matrixSendLatch(14); // Pre-active command
//  matrixSendConfiguration(0x0000, 10);
//  
//  matrixFrameSwap();
  updateMatrix += 1;
}


uint32_t data;
void matrixRefresh(void) {
  uint32_t pos;
  uint32_t bufferPos;
  uint8_t rgb;
    
  if(updateMatrix){
    matrixVerticalSync();
    for(unsigned int y = 0; y < DISPLAY_SCAN_LINES; y++) { 
      for(unsigned int x = 0; x < 16; x++) { 
        bufferPos = y * MATRIX_BUFF_LINE_SIZE + x * BYTE_FOR_PIXEL;
        for(unsigned int sect = 0; sect < MATRIX_NUMBER_CHIPS; sect++) { 
          pos = bufferPos + sect * 16 * BYTE_FOR_PIXEL;
          for(unsigned char bit = 0; bit < 16; bit++) { 
            if (bit < BYTE_FOR_PIXEL){
              rgb = screenBuffer[pos + bit] ;
              RGB_SET(rgb);
            }
            else
              RGB_SET_00();
            
            if(sect == MATRIX_NUMBER_CHIPS - 1 && bit == 15) 
              matrixSetLatHi();//LAT_HIGH; 
            matrixSendClock();
          }
          matrixSetLatLow();// LAT_LOW;
        }
      }
    }
    matrixFrameSwap();
    if(updateMatrix > 0)
      updateMatrix--;
  } 
}




/*****************************END OF FILE****/
