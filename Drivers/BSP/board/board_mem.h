#ifndef __BOARD_MEM_H
#define __BOARD_MEM_H
#include "board_conf.h"

#define RECORD_SIZE									(uint32_t)0x00001000						//Размер записи

#define RECORD_INIT                 (uint16_t)0                     //Здесь записано только номер версии и количество схем настроек (не более 10)
#define RECORD_INIT_SCHEMATIC       (uint16_t)1                     //здесь записаны названия Схем настроек (не более 10)

#define RECORD_SET_UP			          (uint16_t)2						          //Сектор с настройками (адреса,настройки интерфейсов)
#define RECORD_OBJECT               (uint16_t)(RECORD_SET_UP +1)  
#define RECORD_SHIFT_SHEMA          (uint16_t)64                    //Количество секторов под одну схему

#define RECORD_LUT                  (uint16_t)2048                  //Сектор с таблицами LUT
#define RECORD_LUT_COUNT            (uint16_t)128                  //Максимальное количество таблиц 128



uint8_t BSP_Mem_Write(uint32_t record, uint8_t *sourceP, uint32_t len);
uint8_t BSP_Mem_Read(uint32_t record, uint8_t *destP, uint32_t len);
uint8_t BSP_Mem_Erase(uint32_t record);
uint8_t BSP_Mem_Init(void);
void BSP_Mem_Periodic_Handler(void);


#endif /*__BOARD_MEM_H */
