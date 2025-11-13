/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AD5242_H
#define __AD5242_H

/* Includes ------------------------------------------------------------------*/
#include "board_conf.h"
//#include "i2c_application.h"

#define AD5242_OK            			((uint8_t)0x00)
#define AD5242_ERROR         			((uint8_t)0x01)

uint8_t AD5242_Init(void);
uint8_t AD5242_DeInit(void);
/**
  * @brief  Read RDAC
  * @param  devAdr IIC adr DEV_ADR_0/DEV_ADR_1
  * @param  pRxData[2]	(pRxData[0] = RDAC_A, pRxData[0] = RDAC_B)  
  * @retval AD5242_OK  if OK
  *         AD5242_ERROR if error
  */
uint8_t AD5242_Read(uint8_t devAdr, uint8_t * pRxData);

/**
  * @brief  Write RDAC
  * @param  devAdr IIC adr DEV_ADR_0/DEV_ADR_1
  * @return pTxData[2]	(pTxData[0] = RDAC_A, pTxData[0] = RDAC_B)  
  * @retval AD5242_OK  if OK
  *         AD5242_ERROR if error
  */
uint8_t AD5242_Write(uint8_t devAdr, uint8_t * pTxData);

#endif /* __AD5242_H */
