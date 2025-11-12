/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "board_conf.h"
/* UDP local connection port */
#define UDP_SERVER_PORT    7
/* UDP remote connection port */
#define UDP_CLIENT_PORT    7

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   (uint8_t) 192
#define IP_ADDR1   (uint8_t) 168
#define IP_ADDR2   (uint8_t) 0
#define IP_ADDR3   (uint8_t) 10

/*NETMASK*/
#define NETMASK_ADDR0   (uint8_t) 255
#define NETMASK_ADDR1   (uint8_t) 255
#define NETMASK_ADDR2   (uint8_t) 255
#define NETMASK_ADDR3   (uint8_t) 0

/*Gateway Address*/
#define GW_ADDR0   (uint8_t) 192
#define GW_ADDR1   (uint8_t) 168
#define GW_ADDR2   (uint8_t) 0
#define GW_ADDR3   (uint8_t) 1

/* Exported Defines ----------------------------------------------------------*/
#define ID_MODUL 			79
#define ID_DC 				129

#define VERSIA_MIN			(uint8_t)10
#define VERSIA_MAJ			(uint8_t)1
#define VERSIA_MIN_MAJ	(uint8_t)1

/*Настройки для поврежденной или не инициализированной памяти памяти */
#define DEF_MEMINIT   		  (uint32_t)(0x55550a02) 
#define DEF_OBJECTS_COUNT	  0
#define DEF_ADR     			  0xff
#define DEF_AUTO_BRIGHT_ON  0x01     			
#define DEF_BRIGHT     		  50



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

typedef struct{
	uint32_t  MemoryInit;               //
	uint32_t  objectsCount;             //Количество объектов	
  uint32_t  Adr;                      //Adr mod_buss
  uint32_t  autoBrightness;           //Авто яркость
  uint32_t  brightness;               //Яркость
  
}STRUCT_MAIN_SETUP;

typedef struct{
	uint32_t  MemoryInit;
  uint32_t  CurrentSchematic;    //Номер схемы (или номер перегона с данной схемой)
  uint32_t  NumberOfSchematic;  //Всего схем
}STRUCT_INIT;

typedef struct{
	uint32_t  	MemoryInit;
	uint8_t			Name[10][100];
}STRUCT_SCHEMATICS;

extern STRUCT_INIT MainInit;
extern STRUCT_MAIN_SETUP MainSetup;
extern STRUCT_SCHEMATICS MainSchematics;

extern struct MAIN_SR Modul;


void Error_Handler(void);



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
