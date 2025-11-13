/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOARD_LED_H
#define __BOARD_LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Include LED component Driver */
#include "board_conf.h"

typedef enum
{
  LED1 = 0U,
  LED_RED = LED1,
  LED2 = 1U,
  LED_YELLOW = LED2,
  LED3 = 2U,
  LED_GREEN = LED3,
}Led_TypeDef;

uint8_t BSP_LED_Init(void);
uint8_t BSP_LED_On(Led_TypeDef Led);
uint8_t BSP_LED_Off(Led_TypeDef Led);
uint8_t BSP_LED_Toggle(Led_TypeDef Led);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_LED_H */

/*****************************END OF FILE****/
