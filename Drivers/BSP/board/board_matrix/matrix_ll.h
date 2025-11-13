/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOARD_MATRIX_LL_H
#define __BOARD_MATRIX_LL_H
#include "board_conf.h"
#include "regConf_FM6353.h"

#define NOP() __ASM("NOP")

#define R1(bit)			R1_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(R1_GPIO_PIN<<16):(uint32_t)(R1_GPIO_PIN)
#define G1(bit)			G1_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(G1_GPIO_PIN<<16):(uint32_t)(G1_GPIO_PIN)
#define B1(bit)			B1_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(B1_GPIO_PIN<<16):(uint32_t)(B1_GPIO_PIN)
#define R2(bit)			R2_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(R2_GPIO_PIN<<16):(uint32_t)(R2_GPIO_PIN)
#define G2(bit)			G2_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(G2_GPIO_PIN<<16):(uint32_t)(G2_GPIO_PIN)
#define B2(bit)			B2_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(B2_GPIO_PIN<<16):(uint32_t)(B2_GPIO_PIN)
#define R3(bit)			R3_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(R3_GPIO_PIN<<16):(uint32_t)(R3_GPIO_PIN)
#define G3(bit)			G3_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(G3_GPIO_PIN<<16):(uint32_t)(G3_GPIO_PIN)
#define B3(bit)			B3_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(B3_GPIO_PIN<<16):(uint32_t)(B3_GPIO_PIN)
#define R4(bit)			R4_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(R4_GPIO_PIN<<16):(uint32_t)(R4_GPIO_PIN)
#define G4(bit)			G4_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(G4_GPIO_PIN<<16):(uint32_t)(G4_GPIO_PIN)
#define B4(bit)			B4_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(B4_GPIO_PIN<<16):(uint32_t)(B4_GPIO_PIN)

#define A(bit)			A_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(A_GPIO_PIN<<16):(uint32_t)(A_GPIO_PIN)
#define B(bit)			B_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(B_GPIO_PIN<<16):(uint32_t)(B_GPIO_PIN)
#define C(bit)			C_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(C_GPIO_PIN<<16):(uint32_t)(C_GPIO_PIN)
#define D(bit)			D_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(D_GPIO_PIN<<16):(uint32_t)(D_GPIO_PIN)
#define E(bit)			E_GPIO_PORT->scr=((bit) == 0)?(uint32_t)(E_GPIO_PIN<<16):(uint32_t)(E_GPIO_PIN)

#define SCANLINE_SET(num) {                 \
  A_GPIO_PORT->scr = num<<6;                \
  A_GPIO_PORT->clr = (~(num<<6))&0x7C0;      \
}

#define RGB_SET(num) {                      \
  R1_GPIO_PORT->scr = num<<9;               \
  R1_GPIO_PORT->clr = (~(num<<9))&0x7E00;   \
}

#define RGB_SET_FF() {                   \
  R1_GPIO_PORT->scr = 0x7E00;               \
}

#define RGB_SET_00() {                   \
  R1_GPIO_PORT->clr = 0x7E00;               \
}


#define CLK_LOW			CLK_GPIO_PORT->clr = CLK_GPIO_PIN;
#define CLK_HIGH		CLK_GPIO_PORT->scr = CLK_GPIO_PIN;
#define LAT_LOW			LAT_GPIO_PORT->clr = LAT_GPIO_PIN;
#define LAT_HIGH		LAT_GPIO_PORT->scr = LAT_GPIO_PIN;
#define OE_LOW			OE_GPIO_PORT->clr = OE_GPIO_PIN;
#define OE_HIGH			OE_GPIO_PORT->scr = OE_GPIO_PIN;



extern regConf registers[];

void matrixInit(void);
void matrixPwmStart(void);
void matrixVerticalSync(void);
void matrixFrameSwap(void);

void matrixSendVerticalSync(void);
void matrixSendPreAct(void);
void matrixSendOn(void);
void matrixSendOff(void);
void matrixSendConfReg(regConf reg);

void matrixSendClock(void);
void matrixSetLatLow(void);
void matrixSetLatHi(void);
void matrixDelay(uint32_t nops);




//void matrixSendClock(void);
//void matrixSendLatch(uint8_t clocks);
//void matrixSendConfiguration(uint16_t data, uint8_t latches);
//void matrixSetScanLine(uint8_t line);
//






#endif /* __BOARD_MATRIX_LL_H */

/*****************************END OF FILE****/
