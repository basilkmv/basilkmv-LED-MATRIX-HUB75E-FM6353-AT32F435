#include "SystemTimer.h"

__IO uint32_t uwTick;

#define MS_TICK         			(system_core_clock / 1000U)

void SystemTimer_Config(void)
{
	/* config systick clock source */
  systick_clock_source_config(SYSTICK_CLOCK_SOURCE_AHBCLK_NODIV);

  /* config systick reload value and enable interrupt */
  SysTick_Config(MS_TICK);
}

void SystemTimer_IncTick(){
	uwTick += 1;
}

uint32_t SystemTimer_GetTick(void)
{
  return uwTick;
}
