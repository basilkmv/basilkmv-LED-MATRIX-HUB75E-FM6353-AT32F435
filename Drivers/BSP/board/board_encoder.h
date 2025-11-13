#ifndef __BOARD_ENCODER_H
#define __BOARD_ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "board_conf.h"


uint8_t BSP_Encoder_Init(void);
int32_t ENCOD_GetDiff(void);
uint32_t ENCOD_GetStatePress(void);

#ifdef __cplusplus
}
#endif

#endif
