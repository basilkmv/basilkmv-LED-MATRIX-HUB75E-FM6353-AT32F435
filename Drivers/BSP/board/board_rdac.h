/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOARD_RDAC_H
#define __BOARD_RDAC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Include LED component Driver */
#include "board_conf.h"


#define RDAC_OK            			((uint8_t)0x00)
#define RDAC_ERROR         			((uint8_t)0x01)


//uint8_t BSP_RDAC_Init(uint8_t rdac0, uint8_t rdac1);
//void BSP_RDAC_Set(uint8_t rdac0, uint8_t rdac1);
uint8_t BSP_RDAC_Init(uint8_t rdac0);
uint8_t BSP_RDAC_Set_Force(uint8_t rdac0);

void BSP_RDAC_Periodic_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_RDAC_H */

/*****************************END OF FILE****/
