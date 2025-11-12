/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_TIMER_H
#define __SYS_TIMER_H
#include "at32f435_437.h"

void SystemTimer_IncTick(void);
void SystemTimer_Config(void);
uint32_t SystemTimer_GetTick(void);

#endif /* __SYS_TIMER_H */
