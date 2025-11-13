
/* Includes ------------------------------------------------------------------*/
#include "board_light_sensor.h"
#include "../Components/PHOTORESISTOR_I2C/photoresistor_i2c.h"
#include "board_matrix.h"
#include "SystemTimer.h"
#include "main.h"

#define LIGHT_SENSOR_PERIOD			500 
#define MIN_LIGHT  0x10
#define MAX_LIGHT  0xFF

static uint8_t currentValue;
static uint8_t data[1];
static uint32_t time;
static uint8_t state = LIGHT_OK;
static void BSP_LightSensor_Restart(void);

uint8_t BSP_LightSensor_Init(void)
{
	uint8_t res;
	state = LIGHT_ERROR;
	res =  Photoresistor_Init();
	if (res != 0)
		return LIGHT_ERROR;
	res = Photoresistor_Read(PHOTORESISTOR_ADR, data,1);
	if (res != 0)
		return LIGHT_ERROR;
  state = LIGHT_OK;  
	return  LIGHT_OK;
}

static void BSP_LightSensor_Restart(void)
{
	Photoresistor_DeInit();
	BSP_LightSensor_Init();
}

void BSP_LightSensor_Periodic_Handler(void)
{
  uint8_t res;
	uint32_t localTime = SystemTimer_GetTick();
	if(localTime - time > LIGHT_SENSOR_PERIOD)
	{
		if (state != LIGHT_OK)
			BSP_LightSensor_Restart();
    else {
      res = Photoresistor_Read(PHOTORESISTOR_ADR, data,1);
      if (res != 0)
        state = LIGHT_ERROR;
      else{
        //обработка и передача 
        currentValue = data[0];
        state = LIGHT_OK;
        if (Modul.autoBrightness){
          uint8_t res= (currentValue <= MIN_LIGHT) ? 0:((currentValue - MIN_LIGHT) * 100)/(MAX_LIGHT-MIN_LIGHT);

//          uint8_t res = (currentValue * 100)/MAX_LIGHT;
          Modul.brightness = res;
          MatrixSetBrightness(res);
        }
      }  
    } 
    time = localTime;	  
	}
}


/*****************************END OF FILE****/
