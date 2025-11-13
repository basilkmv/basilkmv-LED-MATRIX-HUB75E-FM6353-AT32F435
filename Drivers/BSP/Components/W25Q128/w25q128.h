#ifndef __W25Q128_H
#define __W25Q128_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "board_conf.h"
	
//_______________	

#define W25Q128_OK									0//no error
#define W25Q128_ERR_BUS							1//error of bus
#define W25Q128_BUSY								2//error of bus
#define W25Q128_TIMEOUT							3//error of bus
#define W25Q128_ERR_NODEV						4//no device
#define W25Q128_ERR_NOID						5//wrong ID
#define W25Q128_ERR_OUTOFMEM				6//out of memmory 
#define W25Q128_ERR_OUTOFPAGE				7//out of memmory 

//
//Size in dyte
#define QSPI_FLASH_FIFO_DEPTH            (32*4)

#define W25Q128_PAGE_SIZE						(uint16_t)256
#define W25Q128_SECTOR_SIZE					(uint16_t)4096
#define	W25Q128_SECTOR_NB						(uint16_t)(4096)
#define	W25Q128_SECTOR_IN_BLOCK			(uint16_t)(16)
#define	W25Q128_BLOCK_NB						(uint16_t)(256)
#define W25Q128_SIZE								(uint32_t)(W25Q128_BLOCK_NB*W25Q128_SECTOR_IN_BLOCK*W25Q128_SECTOR_SIZE)
#define W25Q128_MANUF_ID						0xEF
#define W25Q128_DEV_ID							0x17
//Time out in ms
#define W25Q128_TIME_OUT_P					(uint16_t)5 		// Page Programming Time 
#define W25Q128_TIME_OUT_SE					(uint16_t)400 	// Sector Erase Time
#define W25Q128_TIME_OUT_CE					(uint16_t)40000	// Chip Erase Time
#define W25Q128_TIME_OUT_SUSP				(uint16_t)1 		// Suspend Time < 1ms
#define W25Q128_TIME_OUT_RES				(uint16_t)1 		// Resume Time	< 1ms
#define W25Q128_TIME_OUT						(uint16_t)2			//all another



uint8_t W25Q128_Init(void);
uint8_t W25Q128_DeInit(void);
uint8_t W25Q128_ReadSector(uint16_t sector, uint8_t* pData, uint16_t len);
uint8_t W25Q128_EraseSector(uint16_t sector);
uint8_t W25Q128_WriteSector(uint16_t sector, uint8_t* pData, uint16_t len);


#ifdef __cplusplus
}
#endif

#endif

/***************************END OF FILE****/
