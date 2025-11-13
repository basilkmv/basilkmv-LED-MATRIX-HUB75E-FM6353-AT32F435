/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "board_conf.h"

/* Exported Defines ----------------------------------------------------------*/
#define ID_MODUL 			79
#define ID_DC 				129

#define VERSIA_MIN			(uint8_t)10
#define VERSIA_MAJ			(uint8_t)1
#define VERSIA_MIN_MAJ	(uint8_t)1


/* Exported types ------------------------------------------------------------*/
struct MAIN_SR{
	uint8_t		error;				
	uint8_t		ID;
	uint8_t		time_set;
	uint8_t		Adr;
  uint8_t		systemstart;
	uint8_t 	freezing;				//Если подвисло то 1
	uint32_t 	systemrestart;
	uint32_t 	pwrrestart;
  uint32_t  objectsCount;             //Количество объектов	
  uint32_t  autoBrightness;
  uint32_t  brightness;
};

extern struct MAIN_SR Modul;

void Error_Handler(void);

#endif /* __MAIN_H */

/****************************END OF FILE****/
