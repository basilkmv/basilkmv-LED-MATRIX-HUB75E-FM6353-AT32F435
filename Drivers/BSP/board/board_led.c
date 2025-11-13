
/* Includes ------------------------------------------------------------------*/
#include "board_led.h"

#define LED_ON(port,pin)			port->scr = pin;										//hi
#define LED_OFF(port,pin)			port->clr = pin;		                //low
#define LED_TOGGL(port,pin)   if(port->odt & pin)             \
                                LED_OFF(port,pin)             \
                              else                            \
                                LED_ON(port,pin);   

uint8_t BSP_LED_Init(void)
{
  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

	crm_periph_clock_enable(LED_R_GPIO_CLK, TRUE);
  
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

	gpio_init_struct.gpio_pins = LED_R_GPIO_PIN;
  gpio_init(LED_R_GPIO_PORT, &gpio_init_struct);
	LED_ON(LED_R_GPIO_PORT, LED_R_GPIO_PIN);

	gpio_init_struct.gpio_pins = LED_Y_GPIO_PIN;
  gpio_init(LED_Y_GPIO_PORT, &gpio_init_struct);
	LED_ON(LED_Y_GPIO_PORT, LED_Y_GPIO_PIN);

  gpio_init_struct.gpio_pins = LED_G_GPIO_PIN;
  gpio_init(LED_G_GPIO_PORT, &gpio_init_struct);
	LED_ON(LED_G_GPIO_PORT, LED_G_GPIO_PIN);

  return 0;
}

uint8_t BSP_LED_On(Led_TypeDef Led)
{
switch(Led)
	{
		case LED1:
			LED_ON(LED_R_GPIO_PORT, LED_R_GPIO_PIN);
		break;
		case LED2:
			LED_ON(LED_Y_GPIO_PORT, LED_Y_GPIO_PIN);
		break;
		case LED3:
			LED_ON(LED_G_GPIO_PORT, LED_G_GPIO_PIN);	
		break;
		default:
			return 1;
	}
	return 0;
}

uint8_t BSP_LED_Off(Led_TypeDef Led)
{
	switch(Led)
	{
		case LED1:
			LED_OFF(LED_R_GPIO_PORT, LED_R_GPIO_PIN);
		break;
		case LED2:
			LED_OFF(LED_Y_GPIO_PORT, LED_Y_GPIO_PIN);
		break;
		case LED3:
			LED_OFF(LED_G_GPIO_PORT, LED_G_GPIO_PIN);	
		break;
		default:
			return 1;
	}
	return 0;
}

uint8_t BSP_LED_Toggle(Led_TypeDef Led)
{
 	switch(Led)
	{
		case LED1:
			LED_TOGGL(LED_R_GPIO_PORT, LED_R_GPIO_PIN);
		break;
		case LED2:
			LED_TOGGL(LED_Y_GPIO_PORT, LED_Y_GPIO_PIN);
		break;
		case LED3:
			LED_TOGGL(LED_G_GPIO_PORT, LED_G_GPIO_PIN);	
		break;
		default:
			return 1;
	}
	return 0;
}

/*****************************END OF FILE****/
