/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOARD_MATRIX_LL_H
#define __BOARD_MATRIX_LL_H

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


#define CLK_LOW			CLK_GPIO_PORT->scr=(uint32_t)((CLK_GPIO_PIN)<<16)
#define CLK_HIGH		{CLK_GPIO_PORT->scr=(CLK_GPIO_PIN); NOP(); NOP();}
#define LAT_LOW			LAT_GPIO_PORT->scr=(uint32_t)((LAT_GPIO_PIN)<<16)
#define LAT_HIGH		{LAT_GPIO_PORT->scr=(LAT_GPIO_PIN); NOP(); NOP();}
#define OE_LOW			OE_GPIO_PORT->scr=(uint32_t)((OE_GPIO_PIN)<<16)
#define OE_HIGH			{OE_GPIO_PORT->scr=(OE_GPIO_PIN); NOP();}



#endif /* __BOARD_MATRIX_LL_H */

/*****************************END OF FILE****/
