
/* Includes ------------------------------------------------------------------*/
#include "matrix_ll.h"
#include "matrixConfig.h"

#define FPWM          (uint32_t)(6000000)
#define F_VSYNC       (uint8_t)1
#define F_BUSY        (uint8_t)2
#define F_SWAP        (uint8_t)4
#define DELAY_ON_PIN  2


regConf registers[REG_NUM] = {
  {1,REG_1,REG_1,REG_1,4},
  {2,REG_2_R,REG_2_G,REG_2_B,6},
  {3,REG_3_R,REG_3_G,REG_3_B,8},
  {4,REG_4_R,REG_4_G,REG_4_B,10},
};

static uint8_t scanLine = 0;
static volatile uint8_t flag = 0;


static void matrixPwmInit(void);
static void matrixDelaySignal(void);

void matrixInit(void)
{
  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);
  
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

	gpio_init_struct.gpio_pins = R1_GPIO_PIN | G1_GPIO_PIN | B1_GPIO_PIN | R2_GPIO_PIN | G2_GPIO_PIN | B2_GPIO_PIN ;
  gpio_init(R1_GPIO_PORT, &gpio_init_struct);
	
    
  gpio_init_struct.gpio_pins = A_GPIO_PIN | B_GPIO_PIN | C_GPIO_PIN | D_GPIO_PIN | E_GPIO_PIN;
  gpio_init(A_GPIO_PORT, &gpio_init_struct);
  
  gpio_init_struct.gpio_pins = CLK_GPIO_PIN | LAT_GPIO_PIN;
  gpio_init(CLK_GPIO_PORT, &gpio_init_struct);
 
  RGB_SET_00();
  CLK_LOW;
  LAT_LOW;

  matrixPwmInit();

}

void matrixPwmStart(void){
  TMR8->ctrl1_bit.tmren = TRUE;
}
void matrixVerticalSync(){
  flag = F_VSYNC;// | F_BUSY;
  while(flag & F_VSYNC);
}

void matrixFrameSwap(){
  flag = F_SWAP;
}

  
void TMR8_OVF_TMR13_IRQHandler(void){
  if(tmr_flag_get(TMR8, TMR_OVF_FLAG) != RESET){
		tmr_flag_clear(TMR8, TMR_OVF_FLAG);
    SCANLINE_SET(scanLine);
    if (scanLine == 0){
      if (flag&F_VSYNC){ 
        matrixSendVerticalSync();
        matrixSendVerticalSync();
        flag = F_BUSY;
      }
      if ((flag&F_BUSY) == 0){
        matrixSendVerticalSync();
        matrixSendVerticalSync();
      }
      if ((flag&F_SWAP)){
        flag &= ~F_SWAP;
        matrixSendVerticalSync();
      }
    }

    TMR5->cval = 0;
    TMR3->cval = 0;
    TMR5->ctrl1 |= 0x01;
    TMR3->ctrl1 |= 0x01;

    scanLine++;
    if (scanLine == DISPLAY_SCAN_LINES){
      scanLine = 0;
    }
  }
}


#pragma push
#pragma O0
void matrixSendClock(void) {
  CLK_HIGH;
  matrixDelaySignal();
  CLK_LOW;
}

void matrixSetLatLow(void){
  LAT_LOW;
}
void matrixSetLatHi(void){
  LAT_HIGH;
}

void matrixSendConfReg(regConf reg){
  uint8_t chipCont = MATRIX_NUMBER_CHIPS;
  uint8_t bitCount;
  uint8_t latches = 16 - reg.latNum;
  uint16_t dataMask;
  uint16_t data;
  
  while(chipCont--) {
    for(bitCount = 0; bitCount < 16; bitCount++) {
      dataMask = 0x8000 >> bitCount;
      
      //B2G2R2B1G1R1      
      data = 0;
      if (reg.valueR & dataMask)
        data +=  0x09;
      if (reg.valueG & dataMask)
        data +=  0x12;
      if (reg.valueB & dataMask)
        data +=  0x24;
      RGB_SET(data);  

      if(chipCont == 0 && bitCount == latches) LAT_HIGH;
      matrixSendClock();
    }
    LAT_LOW;
  }
}

void matrixSendVerticalSync(void) {
  uint8_t clocks = VSYNC_LAT;
  LAT_HIGH;
  matrixDelaySignal();
  while(clocks--) {
      matrixSendClock();
  }
  LAT_LOW;
}

void matrixSendPreAct(void) {
  uint8_t clocks = 14;
  LAT_HIGH;
  matrixDelaySignal();
  while(clocks--) {
      matrixSendClock();
  }
  LAT_LOW;
}

void matrixSendOn(void) {
  uint8_t clocks = 12;
  LAT_HIGH;
  matrixDelaySignal();
  while(clocks--) {
      matrixSendClock();
  }
  LAT_LOW;
}

void matrixSendOff(void) {
  uint8_t clocks = 13;
  LAT_HIGH;
  matrixDelaySignal();
  while(clocks--) {
      matrixSendClock();
  }
  LAT_LOW;
}



void matrixDelay(uint32_t nops)
{
  while(nops--) NOP();
}

static void matrixDelaySignal(void)
{
  uint32_t nops = DELAY_ON_PIN;
  while(nops--) __ASM("NOP");
}


#pragma pop  


static void matrixPwmInit(void){
  tmr_output_config_type  tmr_oc_init_structure;
  gpio_init_type          gpio_init_struct;
  crm_clocks_freq_type    crm_clocks_freq_struct;
  uint16_t                pr;

  crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_TMR8_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_TMR5_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(OE_GPIO_CLK, TRUE);

  gpio_pin_mux_config(GPIOE, GPIO_PINS_SOURCE4, GPIO_MUX_2);
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_pins = OE_GPIO_PIN;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER; 
  gpio_init(OE_GPIO_PORT, &gpio_init_struct);
  
  //PWM
  //Fpwm = 6 Mhz, Tpwm = 166 ns
  //pr = (uint16_t) ((crm_clocks_freq_struct.apb1_freq * 2) / (FPWM*2) - 1;  
  //FPWM*2 - так как режим TMR_OUTPUT_CONTROL_SWITCH;
  crm_clocks_freq_get(&crm_clocks_freq_struct);
  pr = (uint16_t) ((crm_clocks_freq_struct.apb1_freq * 2 ) / (FPWM*2)) - 1;
  tmr_base_init(TMR3, pr, 0);
  tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR3, TMR_CLOCK_DIV2);
  //PWM out
  tmr_output_default_para_init(&tmr_oc_init_structure);
  tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_SWITCH;//TMR_OUTPUT_CONTROL_PWM_MODE_A;//
  tmr_oc_init_structure.oc_idle_state = FALSE;
  tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;
  tmr_oc_init_structure.oc_output_state = TRUE;
  tmr_output_channel_config(TMR3, TMR_SELECT_CHANNEL_2, &tmr_oc_init_structure);
  tmr_channel_value_set(TMR3, TMR_SELECT_CHANNEL_2, pr/2);
  //Включаем тактирование для ведомого таймера TMR5
  tmr_sub_sync_mode_set(TMR3, TRUE);
  tmr_primary_mode_select(TMR3, TMR_PRIMARY_SEL_C2ORAW);
  tmr_period_buffer_enable(TMR3, TRUE);

  //Счетчик импульсов в внешнего пина 138-3 - зависит от FPWM и оптимизации
  tmr_base_init(TMR5, PWM_PULS-2, 0);
  tmr_cnt_dir_set(TMR5, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR5, TMR_CLOCK_DIV1);
  //Источник тактирования для слейва
  tmr_trigger_input_select(TMR5, TMR_SUB_INPUT_SEL_IS1);
  tmr_sub_mode_select(TMR5, TMR_SUB_EXTERNAL_CLOCK_MODE_A);
  //Прерывание
	tmr_flag_clear(TMR5, TMR_OVF_FLAG);
  tmr_interrupt_enable(TMR5, TMR_OVF_INT, TRUE);

  //Сканирование строк Fscan = FPWM/(IMPULS + 62)
  pr = (uint16_t) ((crm_clocks_freq_struct.apb1_freq * 2) / (FPWM/(PWM_PULS + 62))) - 1;
  tmr_base_init(TMR8, pr,0);
  tmr_cnt_dir_set(TMR8, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR8, TMR_CLOCK_DIV1);
  tmr_flag_clear(TMR8, TMR_OVF_FLAG);
  tmr_interrupt_enable(TMR8, TMR_OVF_INT, TRUE);
 
  nvic_irq_enable(TMR5_GLOBAL_IRQn, 0, 1);
  nvic_irq_enable(TMR8_OVF_TMR13_IRQn, 0, 2);
}


void TMR5_GLOBAL_IRQHandler(void){
	if (TMR5->ists & TMR_OVF_FLAG){//tmr_flagtmr_flag_get(TMR5, TMR_OVF_FLAG) != RESET){
    //stopPwm
    TMR3->ctrl1 &= ~0x01;
    TMR5->ctrl1 &= ~0x01;
    TMR5->ists = ~TMR_OVF_FLAG;
  }
}

/*****************************END OF FILE*********************/
