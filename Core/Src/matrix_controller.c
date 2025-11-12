#include "matrix_controller.h"
#include "main.h"
#include "board_matrix.h"
#include "board_led.h"

#define MATRIX_CONTROLLER_PERIOD   100//ms
#define BUFF_LEN  128*64

static uint16_t buff[BUFF_LEN];
static uint32_t matrixTimer;
static matrix_ctrl_type controller; 
static matrix_cmd_type cmdIn;
static matrix_sate_type stateOut;
static matrix_render_type matrix_render;


static void gpioInit(void);
static void spiInit(void);
static void dmaInit(void);
static void spiStart(void);
static void spiStop(void);
static void spiErrorStop(void);
static void setBusy(void);
static void setNotBusy(void);

static void set_receiving_cmd(void);
static void set_receiving_data(void);
static void set_receiving_data_pixel(void);
static void cmd_received(void);
static void data_received(void);
static void pixel_received(uint16_t color565);


void Matrix_Controller_Periodic_Handler(void){
  uint32_t localtime = SystemTimer_GetTick();
  if(localtime - matrixTimer > MATRIX_CONTROLLER_PERIOD){
    matrixTimer = localtime;  
  }
}

void Matrix_Controller_Init(void){
  gpioInit();
  dmaInit();
  spiInit();
  
  controller.ctrlState = M_IDEL;
  controller.spiState = SPI_IDEL;
  setNotBusy();
}

void MatrixSetNotBusy(void){
  setNotBusy();
}

static void cmd_received(void){
  controller.errorState = CTRL_OK;
  controller.cmd = cmdIn.cmd; 
  switch(cmdIn.cmd){
    case CMD_GET_STATE:
      stateOut.cmd = CMD_GET_STATE;
      stateOut.arg1 = controller.ctrlState;
      stateOut.arg2 = 0;
      stateOut.arg3 = 0;
      stateOut.arg4 = 0;
      stateOut.arg5 = 0;
      setNotBusy();
    break;
    case CMD_SET_WIDTH:
      setNotBusy();
    break;
    case CMD_SET_HIGHT:
      setNotBusy();
    break;
    case CMD_SET_BRIGHT:
      MatrixSetBrightness(cmdIn.arg1);
      setNotBusy();
    break;
    case CMD_DRAW_RECT:
      MatrixSetDrawRect(
        cmdIn.arg1,
        cmdIn.arg2,
        cmdIn.arg3,
        cmdIn.arg4,
        cmdIn.arg5
      );
    break;
    case CMD_DRAW_IMAGE:
      matrix_render.xstart = cmdIn.arg1;
      matrix_render.ystart = cmdIn.arg2;
      matrix_render.xend = cmdIn.arg3;
      matrix_render.yend = cmdIn.arg4;
      matrix_render.counter = cmdIn.arg5;
      setNotBusy();
    break;  
    
    case CMD_DRAW_IMAGE_FORS:
      matrix_render.xstart = cmdIn.arg1;
      matrix_render.ystart = cmdIn.arg2;
      matrix_render.xend = cmdIn.arg3;
      matrix_render.yend = cmdIn.arg4;
      matrix_render.counter = cmdIn.arg5;
      matrix_render.x = cmdIn.arg1;
      matrix_render.y = cmdIn.arg2;
      MatrixSetLock();
      setNotBusy();
    break;  
    default:
      controller.errorState = CTRL_ERROR_NOTCMD;
      setNotBusy();
  }
}


static void data_received(){
  controller.spiState = SPI_IDEL;
  if(controller.errorState == CTRL_OK){
    MatrixSetRenderRect(matrix_render.xstart,
                    matrix_render.ystart,
                    matrix_render.xend,
                    matrix_render.yend,
                    buff);
  }
  else{  
    setNotBusy();
  }
}

static void inline pixel_received(uint16_t color565){
  
  if (matrix_render.counter == 0){
    setNotBusy();
    spiStop();
    MatrixSetUnlock();
    return;
  }
  
  
  MatrixDrawPixel(matrix_render.x,matrix_render.y, color565);
  matrix_render.counter--;
  matrix_render.x++;
  if(matrix_render.x > matrix_render.xend){
    matrix_render.x = matrix_render.xstart;
    matrix_render.y++;
    if(matrix_render.y > matrix_render.yend){
      setNotBusy();
      spiStop();
      MatrixSetUnlock();
      MatrixSetUpdate();
      return;
    }
  }
}

static void setNotBusy(){
  controller.ctrlState = M_IDEL;
  controller.errorState = CTRL_OK;
  MATRIX_SET_NOTBUSY();
}

static void setBusy(){
  controller.ctrlState = M_BUSY;
  MATRIX_SET_BUSY();
}


//Принимаем пакет команды
void MATRIX_DMA_RX_IRQHandler(void)
{
 if(dma_interrupt_flag_get(DMA1_FDT2_FLAG) != RESET)
 {
    dma_flag_clear(DMA1_FDT2_FLAG);
    if(controller.spiState == SPI_RESIVING_CMD){
      spiStop();
      cmd_received();
    }
    else if (controller.spiState == SPI_RESIVING_DATA){
      spiStop();
      data_received();
    }
 }
 if(dma_interrupt_flag_get(DMA1_DTERR2_FLAG) != RESET)
    dma_flag_clear(DMA1_DTERR2_FLAG);
}

//Принимаем данные буфера
void MATRIX_SPI_IRQHandler(void)
{
  if(spi_i2s_interrupt_flag_get(MATRIX_SPI, SPI_I2S_RDBF_FLAG) != RESET)
  {
    BSP_LED_On(LED2);
    pixel_received(MATRIX_SPI->dt);
    BSP_LED_Off(LED2);
  }
}

void MATRIX_CS_IRQHandler(void)
{
  if(MATRIX_CS_PORT->idt & MATRIX_CS_PIN){
    if(controller.spiState != SPI_IDEL)
      spiErrorStop();
  }
  else{
    setBusy(); 
    spiStart();
  }  
  exint_flag_clear(MATRIX_CS_EXINT_LINE);
}

static void  spiStop(void){
  controller.spiState = SPI_IDEL;
  spi_enable(MATRIX_SPI, FALSE);
  dma_channel_enable(MATRIX_DMA_TX, FALSE);
  dma_channel_enable(MATRIX_DMA_RX, FALSE);
}

static void  spiStart(void){
  if (MATRIX_GET_CD())
    set_receiving_cmd();
  else{
    if(controller.cmd == CMD_DRAW_IMAGE)
      set_receiving_data();
    else if(controller.cmd == CMD_DRAW_IMAGE_FORS)
      set_receiving_data_pixel();
  }
  spi_enable(MATRIX_SPI, TRUE);
}

static void spiErrorStop(){
  spiStop();
  controller.ctrlState = M_IDEL;
  controller.errorState = CTRL_ERROR_DMA;
  MATRIX_SET_NOTBUSY();
}



static void set_receiving_cmd(void){

  controller.spiState = SPI_RESIVING_CMD;

  spi_i2s_dma_transmitter_enable(MATRIX_SPI, TRUE);
  spi_i2s_dma_receiver_enable(MATRIX_SPI, TRUE);
  MATRIX_SPI->ctrl1_bit.ora = FALSE;
  
  MATRIX_DMA_TX->dtcnt = CMD_SIZE_HELFWORD;
  MATRIX_DMA_TX->paddr = (uint32_t)&MATRIX_SPI->dt;
  MATRIX_DMA_TX->maddr = (uint32_t)&stateOut;
  
  MATRIX_DMA_RX->dtcnt = CMD_SIZE_HELFWORD;
  MATRIX_DMA_RX->paddr = (uint32_t)&MATRIX_SPI->dt;
  MATRIX_DMA_RX->maddr = (uint32_t)&cmdIn;
  
  dma_flag_clear(DMA1_FDT2_FLAG | DMA1_HDT2_FLAG | DMA1_DTERR2_FLAG);
  dma_interrupt_enable(MATRIX_DMA_RX, DMA_FDT_INT | DMA_DTERR_INT, TRUE);

  dma_channel_enable(MATRIX_DMA_TX, TRUE);
  dma_channel_enable(MATRIX_DMA_RX, TRUE);
}



static void set_receiving_data(void){
  controller.spiState = SPI_RESIVING_DATA;

  spi_i2s_dma_receiver_enable(MATRIX_SPI, TRUE);

  MATRIX_DMA_RX->dtcnt = matrix_render.counter;
  MATRIX_DMA_RX->paddr = (uint32_t)&MATRIX_SPI->dt;
  MATRIX_DMA_RX->maddr = (uint32_t)&buff;

  dma_flag_clear(DMA1_FDT2_FLAG | DMA1_HDT2_FLAG | DMA1_DTERR2_FLAG);
  dma_interrupt_enable(MATRIX_DMA_RX, DMA_FDT_INT | DMA_DTERR_INT, TRUE);

  dma_channel_enable(MATRIX_DMA_RX, TRUE);
}

static void set_receiving_data_pixel(void){
  controller.spiState = SPI_RESIVING_DATA_PIXEL;

  spi_i2s_dma_transmitter_enable(MATRIX_SPI, FALSE);
  spi_i2s_dma_receiver_enable(MATRIX_SPI, FALSE);
  dma_flag_clear(DMA1_FDT2_FLAG | DMA1_HDT2_FLAG | DMA1_DTERR2_FLAG);
  dma_interrupt_enable(MATRIX_DMA_RX, DMA_FDT_INT | DMA_DTERR_INT, FALSE);
  
  MATRIX_SPI->ctrl1_bit.ora = TRUE;

  spi_i2s_interrupt_enable(MATRIX_SPI, SPI_I2S_RDBF_INT, TRUE);
}


static void dmaInit(void)
{
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
  dma_init_type dma_init_struct;
  nvic_irq_enable(DMA1_Channel2_IRQn, 1, 0);

  dma_reset(MATRIX_DMA_TX);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_HIGH;
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init(MATRIX_DMA_TX, &dma_init_struct);

  /* dmamux function enable */
  dmamux_enable(DMA1, TRUE);
  dmamux_init(DMA1MUX_CHANNEL1, DMAMUX_DMAREQ_ID_SPI1_TX);
  
  dma_reset(MATRIX_DMA_RX);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_HIGH;
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init(MATRIX_DMA_RX, &dma_init_struct);

  /* dmamux function enable */
  dmamux_enable(DMA1, TRUE);
  dmamux_init(DMA1MUX_CHANNEL2, DMAMUX_DMAREQ_ID_SPI1_RX);
}


static void spiInit(){
  spi_init_type spi_init_struct;
  spi_default_para_init(&spi_init_struct);

  crm_periph_clock_enable(MARTIX_SPI_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);

  nvic_irq_enable(SPI1_IRQn, 1, 1);

  /* configure param */
  spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
  spi_init_struct.master_slave_mode = SPI_MODE_SLAVE;
  spi_init_struct.frame_bit_num = SPI_FRAME_16BIT;
  spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;
  spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_16;
  spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
  spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
  spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
  spi_init(MATRIX_SPI, &spi_init_struct);
  spi_software_cs_internal_level_set(MATRIX_SPI, SPI_SWCS_INTERNAL_LEVEL_LOW);
}

static void gpioInit(void){
  gpio_init_type gpio_init_struct;
  exint_init_type exint_init_struct;
  
  gpio_default_para_init(&gpio_init_struct);

  crm_periph_clock_enable(MATRIX_SCK_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(MATRIX_MISO_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(MATRIX_MOSI_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(MATRIX_CS_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(MATRIX_CD_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(MATRIX_BUSY_PERIPH_CLOCK, TRUE);

  /* configure the SCK pin */
  gpio_pin_mux_config(MATRIX_SCK_PORT, MATRIX_SCK_PINS_SOURCE, GPIO_MUX_5);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = MATRIX_SCK_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(MATRIX_SCK_PORT, &gpio_init_struct);

  /* configure the MISO pin */
  gpio_pin_mux_config(MATRIX_MISO_PORT, MATRIX_MISO_PINS_SOURCE, GPIO_MUX_5);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = MATRIX_MISO_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(MATRIX_MISO_PORT, &gpio_init_struct);

  /* configure the MOSI pin */
  gpio_pin_mux_config(MATRIX_MOSI_PORT, MATRIX_MOSI_PINS_SOURCE, GPIO_MUX_5);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = MATRIX_MOSI_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(MATRIX_MOSI_PORT, &gpio_init_struct);

  /* configure the CS pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = MATRIX_CS_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  gpio_init(MATRIX_CS_PORT, &gpio_init_struct);
  
  /* configure the CD pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = MATRIX_CD_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  gpio_init(MATRIX_CD_PORT, &gpio_init_struct);
  
  /* configure the BUSY pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pins = MATRIX_BUSY_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(MATRIX_BUSY_PORT, &gpio_init_struct);
  MATRIX_SET_BUSY();
  
  scfg_exint_line_config(MATRIX_CS_SCFG_PORT_SOURCE, MATRIX_CS_SCFG_PINS_SOURCE);
  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = MATRIX_CS_EXINT_LINE;
  exint_init_struct.line_polarity = EXINT_TRIGGER_BOTH_EDGE;
  exint_init(&exint_init_struct);
  
  nvic_irq_enable(EXINT4_IRQn, 1, 2);

}

