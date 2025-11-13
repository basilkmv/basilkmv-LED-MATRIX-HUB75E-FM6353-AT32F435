#include "board_mem.h"
#include "main.h"
#include "../Components/w25q128/w25q128.h"


#define NULL_NAME_LEN 20       
const  uint8_t NULLName[100] = "Станция ";


/*
 * 0 сектор количество перегонов, текущий перегон
 * 1 сектор название перегонов

перегон 1
 * 2 сектор настройка перегона 
 * 3 сектор таблица дц
 * 4 сектор таблица индикации ламп
 * 5 сектор таблица индикации ккп
 * 6 сектор имена су
 * 7 -30 сектор настройка СУ

перегон 2
 * 2+29 сектор настройка перегона 
 * 3+29 сектор таблица дц
 * 4+29 сектор таблица индикации ламп
 * 5+29 сектор таблица индикации ккп
 * 6+29 сектор имена су
 * 7+29 - 30+29 сектор настройка СУ

перегон 10
 * 2+29*9 сектор настройка перегона 
 * 3+29*9 сектор таблица дц
 * 4+29*9 сектор таблица индикации ламп
 * 5+29*9 сектор таблица индикации ккп
 * 6+29*9 сектор имена су
 * 7+29*9 - 30+29*9 сектор настройка СУ

*/
#define MEM_TIMER     1000 //ms
static uint32_t MemErrorCount;
static uint32_t MemTime;

static void MemErrorInit(void);
static void MemErrorSetup(void);

uint8_t BSP_Mem_Init(void){
uint8_t res;	
	res = W25Q128_Init();
  if (res != 0){
		MemErrorInit();
		return res;
  }
	
/*
    *  1.Считываем текущюю схему и общее количество схем
    *
*/
	res = BSP_Mem_Read(RECORD_INIT, (void*)&MainInit, sizeof(STRUCT_INIT));
 	if (res != 0){
		//Ошибка памяти
    MemErrorInit();
		MemErrorSetup();
		return res;
	}
  //Проверяем инициализирована ли память
	if (MainInit.MemoryInit != DEF_MEMINIT){
		//Память не инит
    MemErrorInit();
		MemErrorSetup();
		return 1;
  }
  
/*
    *  1.1. Считываем название схем
    *
*/
  res = BSP_Mem_Read(RECORD_INIT_SCHEMATIC, (void*)&MainSchematics, sizeof(STRUCT_SCHEMATICS));
  if (res != 0 || MainSchematics.MemoryInit != DEF_MEMINIT){
    //Если ошибка памяти или нет инициализации 
    for(uint16_t i = 0; i < 10; i++)
      for(uint16_t j=0; j < NULL_NAME_LEN; j++)
        MainSchematics.Name[i][j] = NULLName[j];
	}
	
/*
    *  2.Считываем настройки схемы
    *
*/
	res = BSP_Mem_Read(RECORD_SET_UP + MainInit.CurrentSchematic * RECORD_SHIFT_SHEMA, (void*)&MainSetup, sizeof(STRUCT_MAIN_SETUP));
	if (res != 0){
		//Ошибка памяти
		MemErrorSetup();
		return res;
	}
	if (MainSetup.MemoryInit != DEF_MEMINIT)
	{
		MemErrorSetup();
		return 1;
	}
	//Ok
	return res=0;
}

//Перед записью стираем сектор
//Записываем все данные подряд
uint8_t BSP_Mem_Write(uint32_t record, uint8_t *sourceP, uint32_t len){
uint8_t res=0;
	if (len <= RECORD_SIZE){
		res = W25Q128_EraseSector(record);
		if (res!=0) 
			return res;
		res = W25Q128_WriteSector(record, sourceP, len);
		if (res!=0) return res;
	}
	else{
		res = W25Q128_ERR_OUTOFMEM;
		return res;
	}
	return res;
}

uint8_t BSP_Mem_Read(uint32_t record, uint8_t *destP, uint32_t len){
uint8_t res=0;
  res = W25Q128_ReadSector(record, destP, len);
  if (res!=0){
     MemErrorCount++;
  }
	return res;
}

uint8_t BSP_Mem_Erase(uint32_t record){
uint8_t res=0;
	res = W25Q128_EraseSector(record);
	return res;
}

void BSP_Mem_Periodic_Handler(void){
uint32_t localtime = SystemTimer_GetTick();
  if(localtime - MemTime > MEM_TIMER){
    if (MemErrorCount > 2){
      //Переинициализация памяти
			W25Q128_DeInit();
      W25Q128_Init();
      MemErrorCount=0;
    }  
    MemTime=localtime;
  }
}

static void MemErrorInit(void){
//Если память вообще отвалилась надо сделать минимальную инициализацию

    MainInit.MemoryInit=DEF_MEMINIT;
    MainInit.CurrentSchematic=0;
    MainInit.NumberOfSchematic=1;
}

static void MemErrorSetup(void){
//Если память отвалилась при чтении/записи настроек
    MainSetup.MemoryInit = DEF_MEMINIT;
    MainSetup.objectsCount = DEF_OBJECTS_COUNT;
    MainSetup.Adr = DEF_ADR;
    MainSetup.autoBrightness = DEF_AUTO_BRIGHT_ON;
    MainSetup.brightness = DEF_BRIGHT;

}

