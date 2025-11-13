/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __REG_FM6353_H
#define __REG_FM6353_H

#ifdef __cplusplus
extern "C" {
#endif

#include "board_conf.h"

#define REG_NUM       4

#define DATA_LAT      1
#define VSYNC_LAT     3
#define ON_LAT        12
#define OFF_LAT       13
#define PREACT_LAT    14
#define PWM_PULS      138






typedef struct _reg{
  uint8_t num;
  uint16_t valueR;
  uint16_t valueG;
  uint16_t valueB;
  uint8_t  latNum; 
}regConf;

/*
regFirst Lat 4
r = g = b = 0x1fB8
[3] - ChromatismOptimi (AberrationOptimize)
[5..4] - PartNumRef
[7..6] Low Gray Dot(LowAshPitsImprove) (0 - 3)
[12..8] Scan line number (32 linnes - 0x1F)
[14] - Clear Bad Dot (StarSwipPoint)

regSecond Lat 6
r = 0x739C(0x539c) g = 0x679c(0x4b9c) b = 0x5f9C(0x439c)
[0] - Clear Enhance (Elimate Shadow)
[9..1] - regGain
[14..10] - Clear Shadow (LowGrayAdjustGrade)

regThird Lat 8
r = g 0x68b6 b = 0x6886
[2] -Clear Shadow Open (LowGrayAdjust)
[7..4] - Low Gray White Balance (LowGrayCompensateGrade)


regFourth Lat 10
r = 0x0A00 g = b = 0x0A70
[14] -  Low Gray White Balance Open (LowGrayCompensate)
[6] - FirstLineSlantsDark
[5..4] - FirstLineSlantsDarkOptimization

*/




#define REG_1       (uint16_t)0x1f00
#define REG_2_R     (uint16_t)0x01ff//0x739C//
#define REG_2_G     (uint16_t)0x01ff//0x679C//
#define REG_2_B     (uint16_t)0x01ff//0x5f9C//
#define REG_3_R     (uint16_t)0x0000//0x68b6//
#define REG_3_G     (uint16_t)0x0000//0x68b6//
#define REG_3_B     (uint16_t)0x0000//0x6886//
#define REG_4_R     (uint16_t)0x0000//0x0A00//
#define REG_4_G     (uint16_t)0x0000//0x0A70//
#define REG_4_B     (uint16_t)0x0000//0x0A70//

#define REG_2_GAIN_MASK   0x03FE


#ifdef __cplusplus
}
#endif

#endif /* __REG_FM6353 */

/*****************************END OF FILE****/
