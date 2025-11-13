/* Includes ------------------------------------------------------------------*/
#include "w25q128.h"
#include "w25q128reg.h"
#include "systemtimer.h"

static qspi_cmd_type qspi_flash_cmd_config;

static uint8_t W25Q128_WaitBusy(uint16_t timeOut);
static uint8_t W25Q128_WriteEnable(uint16_t timeOut);
static uint8_t W25Q128_Reset(void);

static void qspi_flash_cmd_read_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr, uint32_t counter);
static void qspi_flash_cmd_write_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr, uint32_t counter);
static void qspi_flash_cmd_erase_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr);
static void qspi_flash_cmd_wren_config(qspi_cmd_type *qspi_cmd_struct);
static void qspi_flash_cmd_rdsr_config(qspi_cmd_type *qspi_cmd_struct);



uint8_t W25Q128_Init(void)
{
	uint8_t res;
  gpio_init_type gpio_init_struct;

  /* enable the qspi flash clock */
  crm_periph_clock_enable(QSPI_FLASH_QSPI_CLK, TRUE);

  /* enable the pin clock */
  crm_periph_clock_enable(QSPI_FLASH_CS_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_FLASH_CLK_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_FLASH_D0_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_FLASH_D1_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_FLASH_D2_GPIO_CLK, TRUE);
  crm_periph_clock_enable(QSPI_FLASH_D3_GPIO_CLK, TRUE);

  /* configure gpio */
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;

  gpio_init_struct.gpio_pins = QSPI_FLASH_CS_GPIO_PIN;
  gpio_init(QSPI_FLASH_CS_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_FLASH_CLK_GPIO_PIN;
  gpio_init(QSPI_FLASH_CLK_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_FLASH_D0_GPIO_PIN;
  gpio_init(QSPI_FLASH_D0_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_FLASH_D1_GPIO_PIN;
  gpio_init(QSPI_FLASH_D1_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_FLASH_D2_GPIO_PIN;
  gpio_init(QSPI_FLASH_D2_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.gpio_pins = QSPI_FLASH_D3_GPIO_PIN;
  gpio_init(QSPI_FLASH_D3_GPIO_PORT, &gpio_init_struct);

  /* gpio mux config */
  gpio_pin_mux_config(QSPI_FLASH_CS_GPIO_PORT,  QSPI_FLASH_CS_GPIO_PINS_SOURCE,  QSPI_FLASH_CS_GPIO_MUX);
  gpio_pin_mux_config(QSPI_FLASH_CLK_GPIO_PORT, QSPI_FLASH_CLK_GPIO_PINS_SOURCE, QSPI_FLASH_CLK_GPIO_MUX);
  gpio_pin_mux_config(QSPI_FLASH_D0_GPIO_PORT,  QSPI_FLASH_D0_GPIO_PINS_SOURCE,  QSPI_FLASH_D0_GPIO_MUX);
  gpio_pin_mux_config(QSPI_FLASH_D1_GPIO_PORT,  QSPI_FLASH_D1_GPIO_PINS_SOURCE,  QSPI_FLASH_D1_GPIO_MUX);
  gpio_pin_mux_config(QSPI_FLASH_D2_GPIO_PORT,  QSPI_FLASH_D2_GPIO_PINS_SOURCE,  QSPI_FLASH_D2_GPIO_MUX);
  gpio_pin_mux_config(QSPI_FLASH_D3_GPIO_PORT,  QSPI_FLASH_D3_GPIO_PINS_SOURCE,  QSPI_FLASH_D3_GPIO_MUX);

  /* switch to cmd port */
  qspi_xip_enable(QSPI_FLASH_QSPIx, FALSE);

  /* set sclk */
  qspi_clk_division_set(QSPI_FLASH_QSPIx, QSPI_CLK_DIV_4);

  /* set sck idle mode 0 */
  qspi_sck_mode_set(QSPI_FLASH_QSPIx, QSPI_SCK_MODE_0);

  /* set wip in bit 0 */
  qspi_busy_config(QSPI_FLASH_QSPIx, QSPI_BUSY_OFFSET_0);
	
	//res = W25Q128_Reset();
	if (res != W25Q128_OK)
		return res;
	return res;
}

uint8_t W25Q128_DeInit(void)
{
	crm_periph_clock_enable(QSPI_FLASH_QSPI_CLK, FALSE);
	return W25Q128_OK;
}


uint8_t W25Q128_ReadSector(uint16_t sector, uint8_t* pData, uint16_t total_len)
{
	uint32_t i, len = total_len;
	uint32_t  addr = sector * W25Q128_SECTOR_SIZE;

	if(W25Q128_WaitBusy(W25Q128_TIME_OUT_SUSP) != W25Q128_OK)
				return W25Q128_TIMEOUT;
	
  qspi_flash_cmd_read_config(&qspi_flash_cmd_config, addr, total_len);
  qspi_cmd_operation_kick(QSPI_FLASH_QSPIx, &qspi_flash_cmd_config);

  /* read data */
  do
  {
    if(total_len >= QSPI_FLASH_FIFO_DEPTH)
    {
      len = QSPI_FLASH_FIFO_DEPTH;
    }
    else
    {
      len = total_len;
    }
    while(qspi_flag_get(QSPI_FLASH_QSPIx, QSPI_RXFIFORDY_FLAG) == RESET);
    for(i = 0; i < len; ++i)
    {
      *pData++ = qspi_byte_read(QSPI_FLASH_QSPIx);
    }
    total_len -= len;
  }while(total_len);

  /* wait command completed */
  while(qspi_flag_get(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG);

	return W25Q128_OK;
}

uint8_t W25Q128_WriteSector(uint16_t sector, uint8_t* pData, uint16_t total_len)
{
	uint32_t i, len;
	uint32_t  addr = sector * W25Q128_SECTOR_SIZE;

  do
  {
    if (W25Q128_WriteEnable(W25Q128_TIME_OUT) != W25Q128_OK)
			return W25Q128_TIMEOUT;
    /* send up to 256 bytes at one time, and only one page */
    len = (addr / W25Q128_PAGE_SIZE + 1) * W25Q128_PAGE_SIZE - addr;
    if(total_len < len)
      len = total_len;

    qspi_flash_cmd_write_config(&qspi_flash_cmd_config, addr, len);
    qspi_cmd_operation_kick(QSPI_FLASH_QSPIx, &qspi_flash_cmd_config);

    for(i = 0; i < len; ++i)
    {
      while(qspi_flag_get(QSPI_FLASH_QSPIx, QSPI_TXFIFORDY_FLAG) == RESET);
      qspi_byte_write(QSPI_FLASH_QSPIx, *pData++);
    }
    total_len -= len;
    addr += len;

    /* wait command completed */
    while(qspi_flag_get(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG) == RESET);
    qspi_flag_clear(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG);

    if(W25Q128_WaitBusy(W25Q128_TIME_OUT_P) != W25Q128_OK)
				return W25Q128_TIMEOUT;

  }while(total_len);	
	
	return W25Q128_OK;
}

uint8_t W25Q128_EraseSector(uint16_t sector)
{
	uint32_t  sec_addr = sector * W25Q128_SECTOR_SIZE;
	if (W25Q128_WriteEnable(W25Q128_TIME_OUT) != W25Q128_OK)
			return W25Q128_TIMEOUT;

  qspi_flash_cmd_erase_config(&qspi_flash_cmd_config, sec_addr);
  qspi_cmd_operation_kick(QSPI_FLASH_QSPIx, &qspi_flash_cmd_config);

  /* wait command completed */
  while(qspi_flag_get(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG) == RESET);
  qspi_flag_clear(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG);

  return W25Q128_WaitBusy(W25Q128_TIME_OUT_SE);

}

static uint8_t W25Q128_WaitBusy(uint16_t timeOut)
{
	uint32_t Tickstart;
	qspi_flash_cmd_rdsr_config(&qspi_flash_cmd_config);
  qspi_cmd_operation_kick(QSPI_FLASH_QSPIx, &qspi_flash_cmd_config);

  /* wait command completed */
	Tickstart = SystemTimer_GetTick();
  while(qspi_flag_get(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG) == RESET){
		if (SystemTimer_GetTick() - Tickstart > timeOut)
				return  W25Q128_TIMEOUT;
	}
  qspi_flag_clear(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG);
	
	return W25Q128_OK;
}

static uint8_t W25Q128_WriteEnable(uint16_t timeOut)
{
	uint32_t Tickstart;
	qspi_flash_cmd_wren_config(&qspi_flash_cmd_config);
  qspi_cmd_operation_kick(QSPI_FLASH_QSPIx, &qspi_flash_cmd_config);

  /* wait command completed */
	Tickstart = SystemTimer_GetTick();
  while(qspi_flag_get(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG) == RESET){
		if (SystemTimer_GetTick() - Tickstart > timeOut)
				return  W25Q128_TIMEOUT;
	}
  qspi_flag_clear(QSPI_FLASH_QSPIx, QSPI_CMDSTS_FLAG);
	
	return W25Q128_OK;
}
static uint8_t W25Q128_Reset(void)
{
	return W25Q128_OK;
}

/**
  * @brief  qspi_flash cmd read config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @param  addr: read start address
  * @param  counter: read data counter
  * @retval none
  */
static void qspi_flash_cmd_read_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr, uint32_t counter)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0xEB;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = addr;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_3_BYTE;
  qspi_cmd_struct->data_counter = counter;
  qspi_cmd_struct->second_dummy_cycle_num = 6;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_144;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = FALSE;
}

/**
  * @brief  qspi_flash cmd write config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @param  addr: write start address
  * @param  counter: write data counter
  * @retval none
  */
static void qspi_flash_cmd_write_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr, uint32_t counter)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x32;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = addr;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_3_BYTE;
  qspi_cmd_struct->data_counter = counter;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_114;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = TRUE;
}

/**
  * @brief  qspi_flash cmd erase config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @param  addr: erase address
  * @retval none
  */
static void qspi_flash_cmd_erase_config(qspi_cmd_type *qspi_cmd_struct, uint32_t addr)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x20;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = addr;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_3_BYTE;
  qspi_cmd_struct->data_counter = 0;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_111;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = TRUE;
}

/**
  * @brief  qspi_flash cmd wren config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
static void qspi_flash_cmd_wren_config(qspi_cmd_type *qspi_cmd_struct)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x06;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = 0;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_0_BYTE;
  qspi_cmd_struct->data_counter = 0;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_111;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = FALSE;
  qspi_cmd_struct->write_data_enable = TRUE;
}

/**
  * @brief  qspi_flash cmd rdsr config
  * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
  * @retval none
  */
static void qspi_flash_cmd_rdsr_config(qspi_cmd_type *qspi_cmd_struct)
{
  qspi_cmd_struct->pe_mode_enable = FALSE;
  qspi_cmd_struct->pe_mode_operate_code = 0;
  qspi_cmd_struct->instruction_code = 0x05;
  qspi_cmd_struct->instruction_length = QSPI_CMD_INSLEN_1_BYTE;
  qspi_cmd_struct->address_code = 0;
  qspi_cmd_struct->address_length = QSPI_CMD_ADRLEN_0_BYTE;
  qspi_cmd_struct->data_counter = 0;
  qspi_cmd_struct->second_dummy_cycle_num = 0;
  qspi_cmd_struct->operation_mode = QSPI_OPERATE_MODE_111;
  qspi_cmd_struct->read_status_config = QSPI_RSTSC_HW_AUTO;
  qspi_cmd_struct->read_status_enable = TRUE;
  qspi_cmd_struct->write_data_enable = FALSE;
}

//end file
