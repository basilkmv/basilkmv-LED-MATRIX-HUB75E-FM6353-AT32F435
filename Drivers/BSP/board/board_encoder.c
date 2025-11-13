
#include "board_encoder.h"
/* delay macros */

#define LONG_PRESS_TIME   3000

static int16_t curCount;
static int32_t	shiftCount;
static int32_t	pressed = 0;
static int32_t  timeDown;
 


int32_t contIRQ = 0;


uint8_t BSP_Encoder_Init(void){
	
	gpio_init_type  gpio_init_struct = {0};
  exint_init_type exint_init_struct;
	 
	crm_periph_clock_enable(ENCOD_TMR_CLK	, TRUE);
  crm_periph_clock_enable(ENCOD_A_GPIO_CLK, TRUE);
	crm_periph_clock_enable(ENCOD_EXINT_CLK, TRUE);
	
	/* timer inputput pin Configuration */  
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;
	gpio_init_struct.gpio_pins =  ENCOD_A_GPIO_PIN | ENCOD_B_GPIO_PIN;
  gpio_init(ENCOD_A_GPIO_PORT, &gpio_init_struct);

  gpio_pin_mux_config(ENCOD_A_GPIO_PORT, ENCOD_A_GPIO_PINS_SOURCE, GPIO_MUX_2);
  gpio_pin_mux_config(ENCOD_B_GPIO_PORT, ENCOD_B_GPIO_PINS_SOURCE, GPIO_MUX_2);

	/* SW pin Configuration */
	gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init_struct.gpio_pins =  ENCOD_SW_GPIO_PIN;
  gpio_init(ENCOD_SW_GPIO_PORT, &gpio_init_struct);

	scfg_exint_line_config(ENCOD_SW_SCFG_PORT_SOURCE, ENCOD_SW_SCFG_PINS_SOURCE);
	exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = ENCOD_SW_EXINT_LINE;
  exint_init_struct.line_polarity = EXINT_TRIGGER_BOTH_EDGE;
  exint_init(&exint_init_struct);
	
	nvic_irq_enable(ENCOD_IRQn, ENCODER_SW_IT, 0);


/* tmr encoder mode configuration--------------------------
  tmr ti1pf1 ,ti2fp2 as encoder input pin, tmr counter
  changed each signal edge.
  ------------------------------------------------------------ */

  tmr_base_init(ENCOD_TMR, 0xFFFFFFFF, 0);
  tmr_cnt_dir_set(ENCOD_TMR, TMR_COUNT_UP);

  /* config encoder mode */
  tmr_encoder_mode_config(ENCOD_TMR, TMR_ENCODER_MODE_B, TMR_INPUT_RISING_EDGE, TMR_INPUT_RISING_EDGE);

  /* enable TMR */
  tmr_counter_enable(ENCOD_TMR, TRUE);
	return 0;
}

void ENCOD_IRQHandler(void)
{
	if(exint_interrupt_flag_get(ENCOD_SW_EXINT_LINE) != RESET)
  {
    exint_flag_clear(ENCOD_SW_EXINT_LINE);
    if (gpio_input_data_bit_read(ENCOD_SW_GPIO_PORT,ENCOD_SW_GPIO_PIN)){
      pressed = 2; //up
      if(SystemTimer_GetTick() - timeDown >= LONG_PRESS_TIME)
        pressed = 3;
    }
    else{ 
      pressed = 1; //down
      timeDown = SystemTimer_GetTick();  
    }
  }
}

int32_t ENCOD_GetDiff(void)
{
	shiftCount = (int16_t)ENCOD_TMR->cval - (int16_t)curCount;
	curCount =  ENCOD_TMR->cval;
	return shiftCount;
}

uint32_t ENCOD_GetStatePress(void){
  int32_t res = pressed;  
	pressed = 0;
	return res;
}
