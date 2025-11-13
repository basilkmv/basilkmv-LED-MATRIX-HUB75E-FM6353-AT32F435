
/* Includes ------------------------------------------------------------------*/
#include "board_rdac.h"
#include "../Components/AD5242/ad5242.h"
#include "agc.h"
#include "SystemTimer.h"
#define RDAC_PERIOD			200 

static uint8_t current_rdac0;
static uint8_t rdac_data[2];
static uint32_t rdac_time;
static uint8_t rdacState = RDAC_OK;
static void BSP_RDAC_Restart(void);

uint8_t BSP_RDAC_Init(uint8_t rdac0)
{
	uint8_t res;
	rdacState = RDAC_ERROR;
	res =  AD5242_Init();
	if (res != 0)
		return AD5242_ERROR;
	//Set new value device 0
	rdac_data[0] = rdac_data[1] = current_rdac0 = rdac0; 
	res = AD5242_Write(AD5242_ADR, rdac_data);
	if (res != 0)
		return AD5242_ERROR;
	res = AD5242_Read(AD5242_ADR, rdac_data);
	if (res != 0)
		return AD5242_ERROR;
	if (rdac_data[0] != rdac0 || rdac_data[1] != rdac0)
		return AD5242_ERROR;
	Agc.SatrtProcessMetter = 1;
	rdacState = RDAC_OK;
	return  AD5242_OK;
}

static void BSP_RDAC_Restart(void)
{
	AD5242_DeInit();
	BSP_RDAC_Init(current_rdac0);
}

uint8_t BSP_RDAC_Set_Force(uint8_t rdac0)
{
	static uint8_t res;
	rdacState = RDAC_ERROR;
	if(current_rdac0 != rdac0)
	{
		rdac_data[0] = rdac_data[1] = rdac0; 
		res = AD5242_Write(AD5242_ADR, rdac_data);
		if (res == AD5242_OK)
		{
			res = AD5242_Read(AD5242_ADR, rdac_data);
			if (res == AD5242_OK && rdac_data[0] == rdac0 && rdac_data[1] == rdac0)
				current_rdac0 = rdac0;
			else 
				return AD5242_ERROR;
		}
		else 
			return AD5242_ERROR;
	}
	rdacState = RDAC_OK;
	return AD5242_OK;
	
}

void BSP_RDAC_Periodic_Handler(void)
{
	uint32_t localTime = SystemTimer_GetTick();
	if(localTime - rdac_time > RDAC_PERIOD)
	{
		rdac_time = localTime;	
		if (rdacState != RDAC_OK)
			BSP_RDAC_Restart();
	}
}


/*****************************END OF FILE****/
