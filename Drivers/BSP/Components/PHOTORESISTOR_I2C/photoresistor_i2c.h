/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PHOTORESISTOR_H
#define __PHOTORESISTOR_H

/* Includes ------------------------------------------------------------------*/
#include "board_conf.h"

uint8_t Photoresistor_Init(void);
uint8_t Photoresistor_DeInit(void);
/**
  * @brief  Read 
  * @param  devAdr IIC
  * @param  pRxData[]	  
  * @retval I2C_OK  if OK
  *         I2C_ERROR if error
  */
uint8_t Photoresistor_Read(uint8_t devAdr, uint8_t *pRxData, uint8_t len);

/**
  * @brief  Write 
  * @param  devAdr IIC 
  * @return pTxData[]	  
  * @retval I2C_OK  if OK
  *         I2C_ERROR if error
  */
uint8_t Photoresistor_Write(uint8_t devAdr, uint8_t *pTxData, uint8_t len);

#endif /* __PHOTORESISTOR_H */
