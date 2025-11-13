/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOARD_LIGHT_SENSOR_H
#define __BOARD_LIGHT_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Include LED component Driver */
#include "board_conf.h"


#define LIGHT_OK            			((uint8_t)0x00)
#define LIGHT_ERROR         			((uint8_t)0x01)


uint8_t BSP_LightSensor_Init(void);
void BSP_LightSensor_Periodic_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_LIGTH_SENSOR_H */

/*****************************END OF FILE****/
