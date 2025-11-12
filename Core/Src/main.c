
#include "at32f435_437.h"
#include "at32f435_437_clock.h"
#include "main.h"
#include "board_delay.h"
#include "board_led.h"
#include "board_matrix.h"
#include "matrix_controller.h"
#include "SystemTimer.h"

struct MAIN_SR Modul;

static void System_Periodic_Handle(void);
/**
  * @brief  main program
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();
	SystemTimer_Config();
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_1);
  Modul.brightness = 0;
  BSP_LED_Init();
  BSP_LED_Off(LED3);
  Matrix_Init();
  Matrix_Controller_Init();
  BSP_LED_Off(LED2);
	
  while(1)
  {
		System_Periodic_Handle();
  }
}

static void System_Periodic_Handle(void)
{
  Matrix_Periodic_Handler();
  Matrix_Controller_Periodic_Handler();
}

