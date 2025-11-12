/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MATRIX_H
#define __MATRIX_H
#include "board_conf.h"
#include "matrix_controller_cmd.h"




#define MATRIX_DMA_TX                       DMA1_CHANNEL1                       
#define MATRIX_DMA_TX_IRQHandler            DMA1_Channel1_IRQHandler

#define MATRIX_DMA_RX                       DMA1_CHANNEL2       
#define MATRIX_DMA_RX_IRQHandler            DMA1_Channel2_IRQHandler

                

#define MATRIX_SPI                          SPI1
#define MARTIX_SPI_PERIPH_CLOCK             CRM_SPI1_PERIPH_CLOCK
#define MATRIX_SPI_MUX                      GPIO_MUX_5
#define MATRIX_SPI_IRQHandler               SPI1_IRQHandler

#define MATRIX_SCK_PIN											GPIO_PINS_3
#define MATRIX_SCK_PORT 										GPIOB
#define MATRIX_SCK_PINS_SOURCE              GPIO_PINS_SOURCE3
#define MATRIX_SCK_PERIPH_CLOCK          			  CRM_GPIOB_PERIPH_CLOCK

#define MATRIX_MISO_PIN											GPIO_PINS_4
#define MATRIX_MISO_PORT 										GPIOB
#define MATRIX_MISO_PINS_SOURCE             GPIO_PINS_SOURCE4
#define MATRIX_MISO_PERIPH_CLOCK         			  CRM_GPIOB_PERIPH_CLOCK

#define MATRIX_MOSI_PIN											GPIO_PINS_5
#define MATRIX_MOSI_PORT 										GPIOB
#define MATRIX_MOSI_PINS_SOURCE             GPIO_PINS_SOURCE5
#define MATRIX_MOSI_PERIPH_CLOCK        			  CRM_GPIOB_PERIPH_CLOCK

//
#define MATRIX_CS_PIN											  GPIO_PINS_4
#define MATRIX_CS_PORT 										  GPIOA
#define MATRIX_CS_PERIPH_CLOCK     			    CRM_GPIOA_PERIPH_CLOCK
#define MATRIX_CS_SCFG_PORT_SOURCE          SCFG_PORT_SOURCE_GPIOA
#define MATRIX_CS_SCFG_PINS_SOURCE          SCFG_PINS_SOURCE4
#define MATRIX_CS_EXINT_LINE                EXINT_LINE_4
#define MATRIX_CS_IRQHandler                EXINT4_IRQHandler
#define MATRIX_GET_CS()                     MATRIX_CS_PORT->idt & MATRIX_CS_PIN

//1 - cmd 0-data
#define MATRIX_CD_PIN											  GPIO_PINS_5
#define MATRIX_CD_PORT 										  GPIOA
#define MATRIX_CD_PERIPH_CLOCK     		      CRM_GPIOA_PERIPH_CLOCK
#define MATRIX_GET_CD()                     MATRIX_CD_PORT->idt & MATRIX_CD_PIN

//1 busy 0 - redy for transmit
#define MATRIX_BUSY_PIN											GPIO_PINS_6
#define MATRIX_BUSY_PORT 										GPIOA
#define MATRIX_BUSY_PERIPH_CLOCK     			  CRM_GPIOA_PERIPH_CLOCK
#define MATRIX_SET_BUSY()                   MATRIX_BUSY_PORT->scr = MATRIX_BUSY_PIN	  
#define MATRIX_SET_NOTBUSY()                MATRIX_BUSY_PORT->clr = MATRIX_BUSY_PIN


typedef enum{
  M_BUSY = 0,
  M_IDEL = 1,
} ctrl_state_type;

typedef enum{
  SPI_IDEL = 0,
  SPI_RESIVING_CMD = 1,
  SPI_RESIVING_DATA,
  SPI_RESIVING_DATA_PIXEL,
}spi_state_type;

typedef enum{
  CTRL_OK = 0,
  CTRL_ERROR_SPI,
  CTRL_ERROR_DMA,
  CTRL_ERROR_NOTCMD,
}controller_state_type;

typedef struct _matric_controller_{
  ctrl_state_type ctrlState;
  spi_state_type spiState;
  controller_state_type errorState;
  cmd_type cmd;
  uint32_t timer;
  uint16_t xstart;
  uint16_t ystart;
  uint16_t xend;
  uint16_t yend;
} matrix_ctrl_type;

typedef struct {
  uint16_t xstart;
  uint16_t ystart;
  uint16_t xend;
  uint16_t yend;
  uint16_t x;
  uint16_t y;
  uint32_t counter;
  uint32_t ok;
} matrix_render_type;


void Matrix_Controller_Init(void);
void Matrix_Controller_Periodic_Handler(void);
void MatrixSetNotBusy(void);

#endif /* __MATRIX_H */

/*****************************END OF FILE****/
