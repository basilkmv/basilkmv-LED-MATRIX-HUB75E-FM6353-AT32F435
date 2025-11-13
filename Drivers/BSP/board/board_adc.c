#include "board_adc.h"
#include "main.h"
#include "cic.h"
#include "agc.h"

#define F_ADC				28800

#define ADC_BUFF_SIZE			2

static uint32_t adcBuff[2]; 
static int16_t adcVal;
static uint16_t adcValCh0, adcValCh1;

static uint16_t agc_counter = 0;
static uint16_t NumOverage;								//Количество симплов больше Максимально допустимого
static uint16_t NumOptimal;						//Количество симплов в интервале гестирезиса


static void tim_init(void);
static void adc_init(void);
static void DMA_Init(void);
static void adc_dma_tim_start(void);


uint8_t BSP_ADC_Init(void)
{

  gpio_init_type gpio_initstructure;
  gpio_default_para_init(&gpio_initstructure);

	
	//+++++++ Config GPIO ADC	
  crm_periph_clock_enable(ADC_CH1_GPIO_CLK, TRUE);

  /* config adc pin as analog input mode */
  gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
  gpio_initstructure.gpio_pins = ADC_CH1_GPIO_PIN | ADC_CH2_GPIO_PIN;
  gpio_init(ADC_CH1_GPIO_PORT, &gpio_initstructure);
	//+++++++ Config GPIO ADC	
	
	
	tim_init();
	DMA_Init();	
	adc_init();
	
	adc_dma_tim_start();

	return 0;
}

static void tim_init(void)
{
	tmr_output_config_type tmr_oc_init_structure;
  
  crm_periph_clock_enable(ADC_TMR_CLK, TRUE);
	
	/* (systemclock/100)/100 = 28800Hz) */
  tmr_base_init(ADC_TMR, 99,99);
  tmr_cnt_dir_set(ADC_TMR, TMR_COUNT_UP);
  tmr_clock_source_div_set(ADC_TMR, TMR_CLOCK_DIV1);

  tmr_output_default_para_init(&tmr_oc_init_structure);
  tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;
  tmr_oc_init_structure.oc_output_state = TRUE;
  tmr_oc_init_structure.oc_idle_state = FALSE;
  tmr_output_channel_config(ADC_TMR, TMR_SELECT_CHANNEL_1, &tmr_oc_init_structure);
  tmr_channel_value_set(ADC_TMR, TMR_SELECT_CHANNEL_1, 5);

}

static void DMA_Init(void)
{
	dma_init_type dma_init_struct;
  crm_periph_clock_enable(ADC_DMA_CLK, TRUE);
  nvic_irq_enable(ADC_DMA_IRQn, ADC_DMA_IT, 0);

  dma_reset(ADC_DMA_CHANNEL);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size = 2;
  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_base_addr = (uint32_t)adcBuff;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)&(ADCCOM->codt);
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_WORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_HIGH;
  dma_init_struct.loop_mode_enable = TRUE;
  dma_init(ADC_DMA_CHANNEL, &dma_init_struct);

  dmamux_enable(ADC_DMA, TRUE);
  dmamux_init(ADC_DMA_MUX_CHANNEL, DMAMUX_DMAREQ_ID_ADC1);

  /* enable dma transfer complete interrupt */
  dma_interrupt_enable(ADC_DMA_CHANNEL, DMA_FDT_INT  | DMA_HDT_INT, TRUE);
  dma_channel_enable(ADC_DMA_CHANNEL, TRUE);	
}

static void adc_init(void)
{
  adc_common_config_type adc_common_struct;
  adc_base_config_type adc_base_struct;
  crm_periph_clock_enable(ADC1_CLK, TRUE);
  crm_periph_clock_enable(ADC2_CLK, TRUE);
  //nvic_irq_enable(ADC1_2_3_IRQn, 0, 0);
	
	adc_common_default_para_init(&adc_common_struct);
	/* config combine mode */
  adc_common_struct.combine_mode = ADC_ORDINARY_SMLT_ONLY_ONESLAVE_MODE;

  /* config division,adcclk is division by hclk */
  adc_common_struct.div = ADC_HCLK_DIV_4;

  /* config common dma mode,it's useful for ordinary group in combine mode */
  adc_common_struct.common_dma_mode = ADC_COMMON_DMAMODE_2;

  /* config common dma request repeat */
  adc_common_struct.common_dma_request_repeat_state = TRUE;

  /* config adjacent adc sampling interval,it's useful for ordinary shifting mode */
  adc_common_struct.sampling_interval = ADC_SAMPLING_INTERVAL_5CYCLES;

  /* config inner temperature sensor and vintrv */
  adc_common_struct.tempervintrv_state = FALSE;

  /* config voltage battery */
  adc_common_struct.vbat_state = FALSE;
  adc_common_config(&adc_common_struct);

  adc_base_default_para_init(&adc_base_struct);

  adc_base_struct.sequence_mode = TRUE;
  adc_base_struct.repeat_mode = FALSE;
  adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
  adc_base_struct.ordinary_channel_length = 1;
  adc_base_config(ADC1, &adc_base_struct);
  adc_resolution_set(ADC1, ADC_RESOLUTION_12B);

  /* config ordinary channel */
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_1, 1, ADC_SAMPLETIME_12_5);

  /* config ordinary trigger source and trigger edge */
  adc_ordinary_conversion_trigger_set(ADC1, ADC_ORDINARY_TRIG_TMR2CH1, ADC_ORDINARY_TRIG_EDGE_RISING);

  /* config dma mode,it's not useful when common dma mode is use */
  adc_dma_mode_enable(ADC1, FALSE);

  /* config dma request repeat,it's not useful when common dma mode is use */
  adc_dma_request_repeat_enable(ADC1, FALSE);

  /* enable adc overflow interrupt */
  adc_interrupt_enable(ADC1, ADC_OCCO_INT, TRUE);

  adc_base_config(ADC2, &adc_base_struct);
  adc_resolution_set(ADC2, ADC_RESOLUTION_12B);
  adc_ordinary_channel_set(ADC2, ADC_CHANNEL_2, 1, ADC_SAMPLETIME_12_5);
  adc_ordinary_conversion_trigger_set(ADC2, ADC_ORDINARY_TRIG_TMR1CH1, ADC_ORDINARY_TRIG_EDGE_NONE);
  adc_dma_mode_enable(ADC2, FALSE);
  adc_dma_request_repeat_enable(ADC2, FALSE);
  adc_interrupt_enable(ADC2, ADC_OCCO_INT, TRUE);

  /* adc enable */
  adc_enable(ADC1, TRUE);
  adc_enable(ADC2, TRUE);
  while(adc_flag_get(ADC1, ADC_RDY_FLAG) == RESET);
  while(adc_flag_get(ADC2, ADC_RDY_FLAG) == RESET);

  /* adc calibration */
  adc_calibration_init(ADC1);
  while(adc_calibration_init_status_get(ADC1));
  adc_calibration_start(ADC1);
  while(adc_calibration_status_get(ADC1));
  adc_calibration_init(ADC2);
  while(adc_calibration_init_status_get(ADC2));
  adc_calibration_start(ADC2);
  while(adc_calibration_status_get(ADC2));
}

static void adc_dma_tim_start(void)
{
	tmr_channel_enable(ADC_TMR, TMR_SELECT_CHANNEL_1, TRUE);
  tmr_output_enable(ADC_TMR, TRUE);
	tmr_counter_enable(ADC_TMR, TRUE);   
}

/**
  * @brief  this function handles dma1_channel1 handler.
  * @param  none
  * @retval none
  */
void ADC_DMA_IRQHandler(void)
{
  if(dma_flag_get(DMA1_FDT1_FLAG) != RESET)
  {
    dma_flag_clear(DMA1_FDT1_FLAG);
    adcValCh0 = adcBuff[1] & 0xFFFF;
    adcValCh1 = (adcBuff[1] >> 16) & 0xFFFF;
		//adcVal = (adcBuff[1] & 0xFFFF) - ((adcBuff[1] >> 16) & 0xFFFF) ;
		//adcVal = adcValCh0 - ((adcBuff[1] >> 16) & 0xFFFF) ;
    //Cic_Process(adcVal);
  }
	
	if(dma_flag_get(DMA1_HDT1_FLAG) != RESET)
  {
    dma_flag_clear(DMA1_HDT1_FLAG);
    adcValCh0 = adcBuff[0] & 0xFFFF;
    adcValCh1 = (adcBuff[0] >> 16) & 0xFFFF;
		//adcVal = (adcBuff[0] & 0xFFFF) - ((adcBuff[0] >> 16) & 0xFFFF) ;
		//Cic_Process(adcVal);
  }
  
  adcVal = adcValCh0 - adcValCh1;
	Cic_Process(adcVal);
  
	if(Agc.SatrtProcessMetter){
		if (adcValCh0 > MIN_MAX_VALUE){
			//Максимального
			if (adcValCh0 > MAX_VALUE){
				NumOverage++;
			}
			else{
				NumOptimal++;
			}
		}
		agc_counter++;
		if (agc_counter >= AGC_TIMES){
			Agc.StartProcessAgc = 1;			//Запустить расчет
			Agc.SatrtProcessMetter = 0;		//Остановить измерение, перезапустица просле перенастройки коэфю усиления
			Agc.NumOverage = NumOverage;
			Agc.NumOptimal = NumOptimal;
			agc_counter = 0;
			NumOverage = 0;
			NumOptimal = 0;
		}
	}
}

/**
  * @brief  this function handles adc1_2_3 handler.
  * @param  none
  * @retval none
  */
void ADC1_2_3_IRQHandler(void)
{
  if(adc_flag_get(ADC1, ADC_OCCO_FLAG) != RESET)
  {
    adc_flag_clear(ADC1, ADC_OCCO_FLAG);
    
  }
  if(adc_flag_get(ADC2, ADC_OCCO_FLAG) != RESET)
  {
    adc_flag_clear(ADC2, ADC_OCCO_FLAG);
  }
}



//******************************End file************
