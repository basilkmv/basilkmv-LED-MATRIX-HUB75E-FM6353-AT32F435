
#include "at32f435_437.h"
#include "at32f435_437_clock.h"
#include "main.h"
#include "board_delay.h"
#include "board_led.h"
#include "board_matrix.h"
#include "SystemTimer.h"

#define PERIOD  60//ms
uint32_t timer;

struct MAIN_SR Modul;
static void System_Periodic_Handle(void);
static void DoSomething(void);
int main(void)
{
  system_clock_config();
	SystemTimer_Config();
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_1);
  Modul.brightness = 0;
  BSP_LED_Init();
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED2);
  Matrix_Init();
	
  while(1)
  {
		System_Periodic_Handle();
  }
}

static void System_Periodic_Handle(void)
{
  Matrix_Periodic_Handler();
  DoSomething();
}

static void DoSomething(void){
static uint16_t step = 0;
    uint32_t localtime = SystemTimer_GetTick();
    if(localtime - timer > PERIOD){
    switch(step){
      case 0:
       
      break;
      case 1:
      MatrixDrawRect(0, 0, 32, 64,  COLOR_WHITE);
      for(uint16_t i = 0; i<64; i++){
        MatrixDrawPixel(0,i, COLOR_GREEN);
        MatrixDrawPixel(1,i, COLOR_GREEN);
        MatrixDrawPixel(2,i, COLOR_GREEN);
      }
      break;
    
     
      case 2:
      MatrixDrawRect(0, 0, 32, 64,  COLOR_WHITE);
      for(uint16_t i = 0; i<64; i++){
        MatrixDrawPixel(3,i, COLOR_GREEN);
        MatrixDrawPixel(4,i, COLOR_GREEN);
        MatrixDrawPixel(5,i, COLOR_GREEN);
      }
      break;

      case 3:
      MatrixDrawRect(0, 0, 32, 64,  COLOR_WHITE);
      for(uint16_t i = 0; i<64; i++){
        MatrixDrawPixel(6,i, COLOR_YELLOW);
        MatrixDrawPixel(7,i, COLOR_YELLOW);
        MatrixDrawPixel(8,i, COLOR_YELLOW);
      }      
      break;
      
      case 4:
      MatrixDrawRect(0, 0, 32, 64,  COLOR_WHITE);
      for(uint16_t i = 0; i<64; i++){
        MatrixDrawPixel(9,i, COLOR_YELLOW);
        MatrixDrawPixel(10,i, COLOR_YELLOW);
        MatrixDrawPixel(11,i, COLOR_YELLOW);
      }      
      break;
      
      case 5:
      MatrixDrawRect(0, 0, 32, 64,  COLOR_WHITE);
      for(uint16_t i = 0; i<64; i++){
        MatrixDrawPixel(12,i, COLOR_BLACK);
        MatrixDrawPixel(13,i, COLOR_BLACK);
        MatrixDrawPixel(14,i, COLOR_YELLOW);
      }      
      break;
      
      case 6:
      MatrixDrawRect(0, 0, 32, 64,  COLOR_WHITE);
      for(uint16_t i = 0; i<64; i++){
        MatrixDrawPixel(15,i, COLOR_BLUE);
        MatrixDrawPixel(16,i, COLOR_BLUE);
        MatrixDrawPixel(17,i, COLOR_BLUE);
      }      
      break;
      
      case 7:
      MatrixDrawRect(0, 0, 32, 64,  COLOR_WHITE);
      for(uint16_t i = 0; i<64; i++){
        MatrixDrawPixel(18,i, COLOR_RED);
        MatrixDrawPixel(19,i, COLOR_RED);
        MatrixDrawPixel(20,i, COLOR_RED);
      }      
      break;
      case 8:
      MatrixDrawRect(0, 0, 32, 64,  COLOR_WHITE);
      for(uint16_t i = 0; i<64; i++){
        MatrixDrawPixel(21,i, COLOR_RED);
        MatrixDrawPixel(22,i, COLOR_RED);
        MatrixDrawPixel(23,i, COLOR_RED);
      }      
      break;
      case 9:
      MatrixDrawRect(0, 0, 32, 64,  COLOR_WHITE);
      for(uint16_t i = 0; i<64; i++){
        MatrixDrawPixel(24,i, COLOR_RED);
        MatrixDrawPixel(25,i, COLOR_RED);
        MatrixDrawPixel(26,i, COLOR_RED);
      }      
      break;
      
      default:
        step = 0;
    }
    step++;
    timer = localtime;
  }
}

