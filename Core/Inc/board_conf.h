/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "at32f435_437.h"
#include "SystemTimer.h"



/* IRQ priorities */
//ADC DMA
#define ADC_DMA_IT									1U
#define MB_UART_IT									2U
#define MB_TMR_IT										3U
#define DC_UART_IT									4U
#define DC_TMR_IT										5U
#define DATA_UART_IT								6U
#define DATA_TMR_IT									7U

#define ENCODER_SW_IT								8U
#define LCD_DMA1_Channel3_IT				9U
#define OTG_IT											10U



// ModBus 
#define MB_USART														UART7
#define MB_USART_CLK              					CRM_UART7_PERIPH_CLOCK

#define MB_TMR															TMR3
#define MB_TMR_CLK              						CRM_TMR3_PERIPH_CLOCK

#define MB_TX_GPIO_CLK           						CRM_GPIOC_PERIPH_CLOCK
#define MB_TX_GPIO_PIN 											GPIO_PINS_1
#define MB_TX_GPIO_PINS_SOURCE   						GPIO_PINS_SOURCE1
#define MB_TX_GPIO_PORT          						GPIOC
#define MB_TX_GPIO_MUX           						GPIO_MUX_8

#define MB_RX_GPIO_CLK           						CRM_GPIOC_PERIPH_CLOCK
#define MB_RX_GPIO_PIN 											GPIO_PINS_0
#define MB_RX_GPIO_PINS_SOURCE   						GPIO_PINS_SOURCE0
#define MB_RX_GPIO_PORT          						GPIOC
#define MB_RX_GPIO_MUX           						GPIO_MUX_8

#define MB_ON_GPIO_CLK           						CRM_GPIOC_PERIPH_CLOCK
#define MB_ON_GPIO_PIN 											GPIO_PINS_2
#define MB_ON_GPIO_PORT          						GPIOC

#define MB_UART_IRQHandler									UART7_IRQHandler
#define MB_UART_IRQn												UART7_IRQn
#define MB_TMR_IRQHandler										TMR3_GLOBAL_IRQHandler
#define MB_TMR_IRQn													TMR3_GLOBAL_IRQn	


//DC comunication
#define DC_USART														USART3
#define DC_USART_CLK              					CRM_USART3_PERIPH_CLOCK
				
#define DC_TMR															TMR6
#define DC_TMR_CLK              						CRM_TMR6_PERIPH_CLOCK
				
#define DC_TX_GPIO_CLK           						CRM_GPIOC_PERIPH_CLOCK
#define DC_TX_GPIO_PIN 											GPIO_PINS_4
#define DC_TX_GPIO_PINS_SOURCE   						GPIO_PINS_SOURCE4
#define DC_TX_GPIO_PORT          						GPIOC
#define DC_TX_GPIO_MUX           						GPIO_MUX_7
				
#define DC_RX_GPIO_CLK           						CRM_GPIOC_PERIPH_CLOCK
#define DC_RX_GPIO_PIN 											GPIO_PINS_5
#define DC_RX_GPIO_PINS_SOURCE   						GPIO_PINS_SOURCE5
#define DC_RX_GPIO_PORT          						GPIOC
#define DC_RX_GPIO_MUX           						GPIO_MUX_7
				
#define DC_ON_GPIO_CLK           						CRM_GPIOA_PERIPH_CLOCK
#define DC_ON_GPIO_PIN 											GPIO_PINS_7
#define DC_ON_GPIO_PORT          						GPIOA

#define DC_UART_IRQHandler									USART3_IRQHandler
#define DC_UART_IRQn												USART3_IRQn
#define DC_TMR_IRQHandler										TMR6_DAC_GLOBAL_IRQHandler
#define DC_TMR_IRQn													TMR6_DAC_GLOBAL_IRQn	

//Прием данных от КСП
#define DATA_USART													UART4
#define DATA_USART_CLK              				CRM_UART4_PERIPH_CLOCK
                                          
#define DATA_TMR														TMR7
#define DATA_TMR_CLK              					CRM_TMR7_PERIPH_CLOCK
                                          
#define DATA_TX_GPIO_CLK           					CRM_GPIOC_PERIPH_CLOCK
#define DATA_TX_GPIO_PIN 										GPIO_PINS_10
#define DATA_TX_GPIO_PINS_SOURCE   					GPIO_PINS_SOURCE10
#define DATA_TX_GPIO_PORT          					GPIOC
#define DATA_TX_GPIO_MUX           					GPIO_MUX_8
				
#define DATA_RX_GPIO_CLK           					CRM_GPIOC_PERIPH_CLOCK
#define DATA_RX_GPIO_PIN 										GPIO_PINS_11
#define DATA_RX_GPIO_PINS_SOURCE   					GPIO_PINS_SOURCE11
#define DATA_RX_GPIO_PORT          					GPIOC
#define DATA_RX_GPIO_MUX           					GPIO_MUX_8

#define DATA_ON_GPIO_CLK           					CRM_GPIOC_PERIPH_CLOCK
#define DATA_ON_GPIO_PIN 										GPIO_PINS_12
#define DATA_ON_GPIO_PORT          					GPIOC

#define DATA_UART_IRQHandler								UART4_IRQHandler
#define DATA_UART_IRQn											UART4_IRQn
#define DATA_TMR_IRQHandler									TMR7_GLOBAL_IRQHandler
#define DATA_TMR_IRQn												TMR7_GLOBAL_IRQn	


//MEM w25q128
#define QSPI_FLASH_QSPIx                    QSPI1
#define QSPI_FLASH_QSPI_CLK                 CRM_QSPI1_PERIPH_CLOCK

#define QSPI_FLASH_CS_GPIO_CLK              CRM_GPIOB_PERIPH_CLOCK
#define QSPI_FLASH_CS_GPIO_PIN              GPIO_PINS_10
#define QSPI_FLASH_CS_GPIO_PINS_SOURCE      GPIO_PINS_SOURCE10
#define QSPI_FLASH_CS_GPIO_PORT             GPIOB
#define QSPI_FLASH_CS_GPIO_MUX              GPIO_MUX_9

#define QSPI_FLASH_CLK_GPIO_CLK             CRM_GPIOD_PERIPH_CLOCK
#define QSPI_FLASH_CLK_GPIO_PIN             GPIO_PINS_3
#define QSPI_FLASH_CLK_GPIO_PINS_SOURCE     GPIO_PINS_SOURCE3
#define QSPI_FLASH_CLK_GPIO_PORT            GPIOD
#define QSPI_FLASH_CLK_GPIO_MUX             GPIO_MUX_9

#define QSPI_FLASH_D0_GPIO_CLK              CRM_GPIOD_PERIPH_CLOCK
#define QSPI_FLASH_D0_GPIO_PIN              GPIO_PINS_11
#define QSPI_FLASH_D0_GPIO_PINS_SOURCE      GPIO_PINS_SOURCE11
#define QSPI_FLASH_D0_GPIO_PORT             GPIOD
#define QSPI_FLASH_D0_GPIO_MUX              GPIO_MUX_9

#define QSPI_FLASH_D1_GPIO_CLK              CRM_GPIOD_PERIPH_CLOCK
#define QSPI_FLASH_D1_GPIO_PIN              GPIO_PINS_12
#define QSPI_FLASH_D1_GPIO_PINS_SOURCE      GPIO_PINS_SOURCE12
#define QSPI_FLASH_D1_GPIO_PORT             GPIOD
#define QSPI_FLASH_D1_GPIO_MUX              GPIO_MUX_9

#define QSPI_FLASH_D2_GPIO_CLK              CRM_GPIOE_PERIPH_CLOCK
#define QSPI_FLASH_D2_GPIO_PIN              GPIO_PINS_2
#define QSPI_FLASH_D2_GPIO_PINS_SOURCE      GPIO_PINS_SOURCE2
#define QSPI_FLASH_D2_GPIO_PORT             GPIOE
#define QSPI_FLASH_D2_GPIO_MUX              GPIO_MUX_9

#define QSPI_FLASH_D3_GPIO_CLK              CRM_GPIOD_PERIPH_CLOCK
#define QSPI_FLASH_D3_GPIO_PIN              GPIO_PINS_13
#define QSPI_FLASH_D3_GPIO_PINS_SOURCE      GPIO_PINS_SOURCE13
#define QSPI_FLASH_D3_GPIO_PORT             GPIOD
#define QSPI_FLASH_D3_GPIO_MUX              GPIO_MUX_9


////AD5242
//#define AD5242_ADR      										0x58

//#define AD5242_I2C_CLKCTRL_200K             0x30F03C6B   //200K
//#define AD5242_I2C_CLKCTRL_400K             0x10F03D6B   //400K

//#define AD5242_I2C                          I2C1
//#define AD5242_I2C_CLK                      CRM_I2C1_PERIPH_CLOCK

//#define AD5242_I2C_SCL_GPIO_CLK             CRM_GPIOC_PERIPH_CLOCK
//#define AD5242_I2C_SCL_GPIO_PIN             GPIO_PINS_6
//#define AD5242_I2C_SCL_GPIO_PINS_SOURCE     GPIO_PINS_SOURCE6
//#define AD5242_I2C_SCL_GPIO_PORT            GPIOC
//#define AD5242_I2C_SCL_GPIO_MUX             GPIO_MUX_4

//#define AD5242_I2C_SDA_GPIO_CLK             CRM_GPIOC_PERIPH_CLOCK
//#define AD5242_I2C_SDA_GPIO_PIN             GPIO_PINS_7
//#define AD5242_I2C_SDA_GPIO_PINS_SOURCE     GPIO_PINS_SOURCE7
//#define AD5242_I2C_SDA_GPIO_PORT            GPIOC
//#define AD5242_I2C_SDA_GPIO_MUX             GPIO_MUX_4

//Photoresistor
#define PHOTORESISTOR_ADR      										 (0x7C << 1)
#define PHOTORESISTOR_I2C_CLKCTRL_100K             0xC0F03C6B   //100K
#define PHOTORESISTOR_I2C_CLKCTRL_200K             0x30F03C6B   //200K
#define PHOTORESISTOR_I2C_CLKCTRL_400K             0x10F03D6B   //400K
#define PHOTORESISTOR_I2C                          I2C1
#define PHOTORESISTOR_I2C_CLK                      CRM_I2C1_PERIPH_CLOCK
#define PHOTORESISTOR_I2C_SCL_GPIO_CLK             CRM_GPIOC_PERIPH_CLOCK
#define PHOTORESISTOR_I2C_SCL_GPIO_PIN             GPIO_PINS_6
#define PHOTORESISTOR_I2C_SCL_GPIO_PINS_SOURCE     GPIO_PINS_SOURCE6
#define PHOTORESISTOR_I2C_SCL_GPIO_PORT            GPIOC
#define PHOTORESISTOR_I2C_SCL_GPIO_MUX             GPIO_MUX_4
#define PHOTORESISTOR_I2C_SDA_GPIO_CLK             CRM_GPIOC_PERIPH_CLOCK
#define PHOTORESISTOR_I2C_SDA_GPIO_PIN             GPIO_PINS_7
#define PHOTORESISTOR_I2C_SDA_GPIO_PINS_SOURCE     GPIO_PINS_SOURCE7
#define PHOTORESISTOR_I2C_SDA_GPIO_PORT            GPIOC
#define PHOTORESISTOR_I2C_SDA_GPIO_MUX             GPIO_MUX_4

//Encoder
#define ENCOD_A_GPIO_PORT										GPIOB
#define ENCOD_A_GPIO_CLK										CRM_GPIOB_PERIPH_CLOCK	
#define ENCOD_A_GPIO_PIN										GPIO_PINS_6
#define ENCOD_A_GPIO_PINS_SOURCE          	GPIO_PINS_SOURCE6
#define ENCOD_A_GPIO_MUX                  	GPIO_MUX_2

#define ENCOD_B_GPIO_PORT										GPIOB
#define ENCOD_B_GPIO_CLK										CRM_GPIOB_PERIPH_CLOCK	
#define ENCOD_B_GPIO_PIN										GPIO_PINS_7
#define ENCOD_B_GPIO_PINS_SOURCE          	GPIO_PINS_SOURCE7
#define ENCOD_B_GPIO_MUX                  	GPIO_MUX_2

#define ENCOD_SW_GPIO_PORT									GPIOB
#define ENCOD_SW_GPIO_CLK										CRM_GPIOB_PERIPH_CLOCK	
#define ENCOD_SW_GPIO_PIN										GPIO_PINS_8
#define ENCOD_SW_SCFG_PINS_SOURCE						SCFG_PINS_SOURCE8
#define ENCOD_SW_SCFG_PORT_SOURCE						SCFG_PORT_SOURCE_GPIOB
#define ENCOD_SW_EXINT_LINE									EXINT_LINE_8

#define ENCOD_TMR_CLK									 			CRM_TMR4_PERIPH_CLOCK	
#define ENCOD_TMR                  					TMR4

#define ENCOD_EXINT_CLK									 		CRM_SCFG_PERIPH_CLOCK	
#define ENCOD_IRQHandler										EXINT9_5_IRQHandler
#define ENCOD_IRQn													EXINT9_5_IRQn

//ADC
#define ADC																	ADC1
#define ADC1_CLK			             					CRM_ADC1_PERIPH_CLOCK
#define ADC2_CLK			             					CRM_ADC2_PERIPH_CLOCK

#define ADC_CH1_GPIO_PIN										GPIO_PINS_1
#define ADC_CH1_GPIO_CLK              			CRM_GPIOA_PERIPH_CLOCK
#define ADC_CH1_GPIO_PORT										GPIOA

#define ADC_CH2_GPIO_PIN										GPIO_PINS_2
#define ADC_CH2_GPIO_CLK              			CRM_GPIOA_PERIPH_CLOCK
#define ADC_CH2_GPIO_PORT										GPIOA

#define ADC_TMR_CLK									 				CRM_TMR2_PERIPH_CLOCK	
#define ADC_TMR                  						TMR2

#define ADC_DMA_CLK													CRM_DMA1_PERIPH_CLOCK
#define ADC_DMA_CHANNEL											DMA1_CHANNEL1
#define ADC_DMA															DMA1
#define ADC_DMA_MUX_CHANNEL									DMA1MUX_CHANNEL1 

#define ADC_DMA_IRQHandler									DMA1_Channel1_IRQHandler
#define ADC_DMA_IRQn												DMA1_Channel1_IRQn

//LCD
#define LCD_DMA_CLK													CRM_DMA1_PERIPH_CLOCK
#define LCD_DMA_CHANNEL											DMA1_CHANNEL3
#define LCD_DMA															DMA1
#define LCD_DMA_MUX_CHANNEL									DMA1MUX_CHANNEL3 
#define LCD_DMA_IRQHandler									DMA1_Channel3_IRQHandler
#define LCD_DMA_IRQn												DMA1_Channel3_IRQn



#define OTG_CLOCK                           CRM_OTGFS1_PERIPH_CLOCK
#define OTG_IRQn                            OTGFS1_IRQn
#define OTG_IRQHandler                      OTGFS1_IRQHandler
#define OTG_WKUP_IRQ                        OTGFS1_WKUP_IRQn
#define OTG_WKUP_HANDLER                    OTGFS1_WKUP_IRQHandler
#define OTG_WKUP_EXINT_LINE                 EXINT_LINE_18

#define OTG_PIN_GPIO                        GPIOA
#define OTG_PIN_GPIO_CLOCK                  CRM_GPIOA_PERIPH_CLOCK

#define OTG_PIN_DP                          GPIO_PINS_12
#define OTG_PIN_DP_SOURCE                   GPIO_PINS_SOURCE12

#define OTG_PIN_DM                          GPIO_PINS_11
#define OTG_PIN_DM_SOURCE                   GPIO_PINS_SOURCE11

#define OTG_PIN_VBUS                        GPIO_PINS_9
#define OTG_PIN_VBUS_SOURCE                 GPIO_PINS_SOURCE9

#define OTG_PIN_ID                          GPIO_PINS_10
#define OTG_PIN_ID_SOURCE                   GPIO_PINS_SOURCE10

#define OTG_PIN_SOF_GPIO                    GPIOA
#define OTG_PIN_SOF_GPIO_CLOCK              CRM_GPIOB_PERIPH_CLOCK
#define OTG_PIN_SOF                         GPIO_PINS_8
#define OTG_PIN_SOF_SOURCE                  GPIO_PINS_SOURCE8

#define OTG_PIN_MUX                         GPIO_MUX_10


// TP
#define TP_1_GPIO_PIN											  GPIO_PINS_0
#define TP_1_GPIO_PORT 										  GPIOB
#define TP_1_GPIO_CLK             			    CRM_GPIOB_PERIPH_CLOCK
#define TP_2_GPIO_PIN 											GPIO_PINS_1
#define TP_2_GPIO_PORT 										  GPIOB
#define TP_2_GPIO_CLK             			    CRM_GPIOB_PERIPH_CLOCK
#define TP_3_GPIO_PIN 											GPIO_PINS_2
#define TP_3_GPIO_PORT 										  GPIOB
#define TP_3_GPIO_CLK             			    CRM_GPIOB_PERIPH_CLOCK

//LED
#define LED_R_GPIO_PIN											GPIO_PINS_0
#define LED_R_GPIO_PORT 										GPIOB
#define LED_R_GPIO_CLK             			    CRM_GPIOB_PERIPH_CLOCK
#define LED_Y_GPIO_PIN 											GPIO_PINS_1
#define LED_Y_GPIO_PORT 										GPIOB
#define LED_Y_GPIO_CLK             			    CRM_GPIOB_PERIPH_CLOCK
#define LED_G_GPIO_PIN 											GPIO_PINS_2
#define LED_G_GPIO_PORT 										GPIOB
#define LED_G_GPIO_CLK             			    CRM_GPIOB_PERIPH_CLOCK


// MATRIX PORT - CONFIGURE PINS

//color bits 1 - subpanel 1
#define R3_GPIO_PORT                        GPIOD  
#define R3_GPIO_PIN	                        GPIO_PINS_14
#define R3_GPIO_CLK                         CRM_GPIOD_PERIPH_CLOCK
#define G3_GPIO_PORT                        GPIOD
#define G3_GPIO_PIN                         GPIO_PINS_15
#define G3_GPIO_CLK                         CRM_GPIOD_PERIPH_CLOCK
#define B3_GPIO_PORT                        GPIOD
#define B3_GPIO_PIN                         GPIO_PINS_0
#define B3_GPIO_CLK                         CRM_GPIOD_PERIPH_CLOCK

//color bits 2 - subpanel 2
#define R4_GPIO_PORT                        GPIOD  
#define R4_GPIO_PIN	                        GPIO_PINS_1
#define R4_GPIO_CLK                         CRM_GPIOD_PERIPH_CLOCK
#define G4_GPIO_PORT                        GPIOE
#define G4_GPIO_PIN                         GPIO_PINS_7
#define G4_GPIO_CLK                         CRM_GPIOE_PERIPH_CLOCK
#define B4_GPIO_PORT                        GPIOE
#define B4_GPIO_PIN                         GPIO_PINS_8
#define B4_GPIO_CLK                         CRM_GPIOE_PERIPH_CLOCK

//color bits 3 - subpanel 3
#define R1_GPIO_PORT                        GPIOE  
#define R1_GPIO_PIN	                        GPIO_PINS_9
#define R1_GPIO_CLK                         CRM_GPIOE_PERIPH_CLOCK
#define G1_GPIO_PORT                        GPIOE
#define G1_GPIO_PIN                         GPIO_PINS_10
#define G1_GPIO_CLK                         CRM_GPIOE_PERIPH_CLOCK
#define B1_GPIO_PORT                        GPIOE
#define B1_GPIO_PIN                         GPIO_PINS_11
#define B1_GPIO_CLK                         CRM_GPIOE_PERIPH_CLOCK

//color bits 4 - subpanel 4
#define R2_GPIO_PORT                        GPIOE  
#define R2_GPIO_PIN	                        GPIO_PINS_12
#define R2_GPIO_CLK                         CRM_GPIOE_PERIPH_CLOCK
#define G2_GPIO_PORT                        GPIOE
#define G2_GPIO_PIN                         GPIO_PINS_13
#define G2_GPIO_CLK                         CRM_GPIOE_PERIPH_CLOCK
#define B2_GPIO_PORT                        GPIOE
#define B2_GPIO_PIN                         GPIO_PINS_14
#define B2_GPIO_CLK                         CRM_GPIOE_PERIPH_CLOCK

//selection bits
#define A_GPIO_PORT                         GPIOD
#define A_GPIO_PIN                          GPIO_PINS_6
#define A_GPIO_CLK                          CRM_GPIOD_PERIPH_CLOCK
#define B_GPIO_PORT                         GPIOD
#define B_GPIO_PIN		                      GPIO_PINS_7
#define B_GPIO_CLK                          CRM_GPIOD_PERIPH_CLOCK
#define C_GPIO_PORT                         GPIOD
#define C_GPIO_PIN		                      GPIO_PINS_8
#define C_GPIO_CLK                          CRM_GPIOD_PERIPH_CLOCK
#define D_GPIO_PORT                         GPIOD
#define D_GPIO_PIN		                      GPIO_PINS_9
#define D_GPIO_CLK                          CRM_GPIOD_PERIPH_CLOCK
#define E_GPIO_PORT                         GPIOD
#define E_GPIO_PIN		                      GPIO_PINS_10
#define E_GPIO_CLK                          CRM_GPIOD_PERIPH_CLOCK



//control bits
#define CLK_GPIO_PORT                       GPIOD          
#define CLK_GPIO_PIN		                    GPIO_PINS_4
#define CLK_GPIO_CLK                        CRM_GPIOD_PERIPH_CLOCK
#define LAT_GPIO_PORT                       GPIOD
#define LAT_GPIO_PIN		                    GPIO_PINS_5
#define LAT_GPIO_CLK                        CRM_GPIOD_PERIPH_CLOCK
#define OE_GPIO_PORT                        GPIOE
#define OE_GPIO_PIN		                      GPIO_PINS_4
#define OE_GPIO_CLK                         CRM_GPIOE_PERIPH_CLOCK

#define OE_IN_GPIO_PORT                     GPIOB
#define OE_IN_GPIO_PIN		                  GPIO_PINS_14
#define OE_IN_GPIO_CLK                      CRM_GPIOB_PERIPH_CLOCK






#ifdef __cplusplus
}
#endif

#endif /* __BOARD_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
