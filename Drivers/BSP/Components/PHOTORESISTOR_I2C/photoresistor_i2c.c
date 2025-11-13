 /* Includes ------------------------------------------------------------------*/
#include "photoresistor_i2c.h"

#define PH_TIMEOUT                2//ms

/** @defgroup I2C_library_event_check_flag
  * @{
  */

#define I2C_EVENT_CHECK_NONE             ((uint32_t)0x00000000)    /*!< check flag none */
#define I2C_EVENT_CHECK_ACKFAIL          ((uint32_t)0x00000001)    /*!< check flag ackfail */
#define I2C_EVENT_CHECK_STOP             ((uint32_t)0x00000002)    /*!< check flag stop */


/** @defgroup I2C_library_status_code
  * @{
  */
typedef enum
{
  I2C_OK = 0,          /*!< no error */
  I2C_ERR_STEP_1,      /*!< step 1 error */
  I2C_ERR_STEP_2,      /*!< step 2 error */
  I2C_ERR_STEP_3,      /*!< step 3 error */
  I2C_ERR_STEP_4,      /*!< step 4 error */
  I2C_ERR_STEP_5,      /*!< step 5 error */
  I2C_ERR_STEP_6,      /*!< step 6 error */
  I2C_ERR_STEP_7,      /*!< step 7 error */
  I2C_ERR_STEP_8,      /*!< step 8 error */
  I2C_ERR_STEP_9,      /*!< step 9 error */
  I2C_ERR_STEP_10,     /*!< step 10 error */
  I2C_ERR_STEP_11,     /*!< step 11 error */
  I2C_ERR_STEP_12,     /*!< step 12 error */
  I2C_ERR_TCRLD,       /*!< tcrld error */
  I2C_ERR_TDC,         /*!< tdc error */
  I2C_ERR_ADDR,        /*!< addr error */
  I2C_ERR_STOP,        /*!< stop error */
  I2C_ERR_ACKFAIL,     /*!< ackfail error */
  I2C_ERR_TIMEOUT,     /*!< timeout error */
  I2C_ERR_INTERRUPT,   /*!< interrupt error */
  I2C_ERR,
} i2c_status_type;

/** @defgroup I2C_library_handler
  * @{
  */
typedef struct
{
  i2c_type                               *i2cx;                   /*!< i2c registers base address      */
  uint8_t                                *pbuff;                  /*!< pointer to i2c transfer buffer  */
  __IO uint16_t                          psize;                   /*!< i2c transfer size               */
  __IO uint16_t                          pcount;                  /*!< i2c transfer counter            */
  __IO uint32_t                          mode;                    /*!< i2c communication mode          */
  __IO uint32_t                          status;                  /*!< i2c communication status        */
  __IO i2c_status_type                   error_code;              /*!< i2c error code                  */
} i2c_handle_type;


static i2c_handle_type hi2c_ee;

static void i2c_lowlevel_init(i2c_handle_type* hi2c, uint32_t i2c_speed);
static i2c_status_type i2c_master_transmit(i2c_handle_type* hi2c, uint16_t address, uint8_t* pdata, uint16_t size, uint32_t timeout);
static i2c_status_type i2c_master_receive(i2c_handle_type* hi2c, uint16_t address, uint8_t* pdata, uint16_t size, uint32_t timeout);
static void i2c_start_transfer(i2c_handle_type* hi2c, uint16_t address, i2c_start_mode_type start);
static i2c_status_type i2c_wait_flag(i2c_handle_type* hi2c, uint32_t flag, uint32_t event_check, uint32_t timeout);
static void i2c_reset_ctrl2_register(i2c_handle_type* hi2c);

uint8_t Photoresistor_Init(void)
{ 
	
	hi2c_ee.i2cx = PHOTORESISTOR_I2C;

  /* reset i2c peripheral */
  i2c_reset(hi2c_ee.i2cx);

  /* i2c peripheral initialization */
  i2c_lowlevel_init(&hi2c_ee, PHOTORESISTOR_I2C_CLKCTRL_200K);

  /* i2c peripheral enable */
  i2c_enable(hi2c_ee.i2cx, TRUE);

	return I2C_OK;
}

uint8_t Photoresistor_DeInit(void)
{
  crm_periph_clock_enable(PHOTORESISTOR_I2C_CLK, FALSE);
	return I2C_OK;
}

uint8_t Photoresistor_Write(uint8_t devAdr, uint8_t *pTxData, uint8_t len)
{
	uint8_t res;
	res = i2c_master_transmit(&hi2c_ee, devAdr, pTxData, len, PH_TIMEOUT);
	return res;
}

uint8_t Photoresistor_Read(uint8_t devAdr, uint8_t *pRxData, uint8_t len)
{
	uint8_t res;
  res = i2c_master_receive(&hi2c_ee, devAdr, pRxData, len, PH_TIMEOUT);
	return res;
}


static void i2c_lowlevel_init(i2c_handle_type* hi2c, uint32_t i2c_speed)
{
  gpio_init_type gpio_init_structure;

  /* i2c periph clock enable */
  crm_periph_clock_enable(PHOTORESISTOR_I2C_CLK, TRUE);
  crm_periph_clock_enable(PHOTORESISTOR_I2C_SCL_GPIO_CLK, TRUE);
  crm_periph_clock_enable(PHOTORESISTOR_I2C_SDA_GPIO_CLK, TRUE);

  /* gpio configuration */
  gpio_pin_mux_config(PHOTORESISTOR_I2C_SCL_GPIO_PORT, PHOTORESISTOR_I2C_SCL_GPIO_PINS_SOURCE, PHOTORESISTOR_I2C_SCL_GPIO_MUX);

  gpio_pin_mux_config(PHOTORESISTOR_I2C_SDA_GPIO_PORT, PHOTORESISTOR_I2C_SDA_GPIO_PINS_SOURCE, PHOTORESISTOR_I2C_SDA_GPIO_MUX);

  /* configure i2c pins: scl */
  gpio_init_structure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_structure.gpio_mode           = GPIO_MODE_MUX;
  gpio_init_structure.gpio_out_type       = GPIO_OUTPUT_OPEN_DRAIN;
  gpio_init_structure.gpio_pull           = GPIO_PULL_UP;

  gpio_init_structure.gpio_pins           = PHOTORESISTOR_I2C_SCL_GPIO_PIN;
  gpio_init(PHOTORESISTOR_I2C_SCL_GPIO_PORT, &gpio_init_structure);

  /* configure i2c pins: sda */
  gpio_init_structure.gpio_pins           = PHOTORESISTOR_I2C_SDA_GPIO_PIN;
  gpio_init(PHOTORESISTOR_I2C_SDA_GPIO_PORT, &gpio_init_structure);

  /* config i2c */
  i2c_init(hi2c->i2cx, 0x0F, i2c_speed);
//  i2c_own_address1_set(hi2c->i2cx, I2C_ADDRESS_MODE_7BIT, 0x0);
//  i2c_ack_enable(hi2c->i2cx, TRUE);
//  i2c_clock_stretch_enable(hi2c->i2cx, TRUE);
//  i2c_general_call_enable(hi2c->i2cx, FALSE);
}

/**
  * @brief  the master transmits data through polling mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: slave address.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
static i2c_status_type i2c_master_transmit(i2c_handle_type* hi2c, uint16_t address, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->pbuff = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if (i2c_wait_flag(hi2c, I2C_BUSYF_FLAG, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* start transfer */
  i2c_start_transfer(hi2c, address, I2C_GEN_START_WRITE);

  while (hi2c->pcount > 0)
  {
    /* wait for the tdis flag to be set */
    if(i2c_wait_flag(hi2c, I2C_TDIS_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
    {
      return I2C_ERR_STEP_2;
    }

    /* send data */
    i2c_data_send(hi2c->i2cx, *hi2c->pbuff++);
    hi2c->psize--;
    hi2c->pcount--;

    if ((hi2c->psize == 0) && (hi2c->pcount != 0))
    {
      /* wait for the tcrld flag to be set  */
      if (i2c_wait_flag(hi2c, I2C_TCRLD_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
      {
        return I2C_ERR_STEP_3;
      }

      /* continue transfer */
      i2c_start_transfer(hi2c, address, I2C_WITHOUT_START);
    }
  }

  /* wait for the stop flag to be set  */
  if(i2c_wait_flag(hi2c, I2C_STOPF_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_4;
  }

  /* clear stop flag */
  i2c_flag_clear(hi2c->i2cx, I2C_STOPF_FLAG);

  /* reset ctrl2 register */
  i2c_reset_ctrl2_register(hi2c);

  return I2C_OK;
}

/**
  * @brief  the master receive data through polling mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: slave address.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
static i2c_status_type i2c_master_receive(i2c_handle_type* hi2c, uint16_t address, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->pbuff = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if (i2c_wait_flag(hi2c, I2C_BUSYF_FLAG, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* start transfer */
  i2c_start_transfer(hi2c, address, I2C_GEN_START_READ);

  while (hi2c->pcount > 0)
  {
    /* wait for the rdbf flag to be set  */
    if(i2c_wait_flag(hi2c, I2C_RDBF_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
    {
      return I2C_ERR_STEP_2;
    }

    /* read data */
    (*hi2c->pbuff++) = i2c_data_receive(hi2c->i2cx);
    hi2c->pcount--;
    hi2c->psize--;

    if ((hi2c->psize == 0) && (hi2c->pcount != 0))
    {
      /* wait for the tcrld flag to be set  */
      if (i2c_wait_flag(hi2c, I2C_TCRLD_FLAG, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
      {
        return I2C_ERR_STEP_3;
      }

      /* continue transfer */
      i2c_start_transfer(hi2c, address, I2C_WITHOUT_START);
    }
  }

  /* wait for the stop flag to be set  */
  if(i2c_wait_flag(hi2c, I2C_STOPF_FLAG, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_4;
  }

  /* clear stop flag */
  i2c_flag_clear(hi2c->i2cx, I2C_STOPF_FLAG);

  /* reset ctrl2 register */
  i2c_reset_ctrl2_register(hi2c);

  return I2C_OK;
}

/**
  * @brief  start transfer in poll mode or interrupt mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: slave address.
  * @param  start: config gen start condition mode.
  *         parameter as following values:
  *         - I2C_WITHOUT_START: transfer data without start condition.
  *         - I2C_GEN_START_READ: read data and generate start.
  *         - I2C_GEN_START_WRITE: send data and generate start.
  * @retval i2c status.
  */
static void i2c_start_transfer(i2c_handle_type* hi2c, uint16_t address, i2c_start_mode_type start)
{
  if (hi2c->pcount > MAX_TRANSFER_CNT)
  {
    hi2c->psize = MAX_TRANSFER_CNT;

    i2c_transmit_set(hi2c->i2cx, address, hi2c->psize, I2C_RELOAD_MODE, start);
  }
  else
  {
    hi2c->psize = hi2c->pcount;

    i2c_transmit_set(hi2c->i2cx, address, hi2c->psize, I2C_AUTO_STOP_MODE, start);
  }
}

/**
  * @brief  wait for the flag to be set or reset, only BUSYF flag
  *         is waiting to be reset, and other flags are waiting to be set
  * @param  hi2c: the handle points to the operation information.
  * @param  flag: specifies the flag to check.
  *         this parameter can be one of the following values:
  *         - I2C_TDBE_FLAG: transmit data buffer empty flag.
  *         - I2C_TDIS_FLAG: send interrupt status.
  *         - I2C_RDBF_FLAG: receive data buffer full flag.
  *         - I2C_ADDRF_FLAG: 0~7 bit address match flag.
  *         - I2C_ACKFAIL_FLAG: acknowledge failure flag.
  *         - I2C_STOPF_FLAG: stop condition generation complete flag.
  *         - I2C_TDC_FLAG: transmit data complete flag.
  *         - I2C_TCRLD_FLAG: transmission is complete, waiting to load data.
  *         - I2C_BUSERR_FLAG: bus error flag.
  *         - I2C_ARLOST_FLAG: arbitration lost flag.
  *         - I2C_OUF_FLAG: overflow or underflow flag.
  *         - I2C_PECERR_FLAG: pec receive error flag.
  *         - I2C_TMOUT_FLAG: smbus timeout flag.
  *         - I2C_ALERTF_FLAG: smbus alert flag.
  *         - I2C_BUSYF_FLAG: bus busy flag transmission mode.
  *         - I2C_SDIR_FLAG: slave data transmit direction.
  * @param  event_check: check other error flags while waiting for the flag.
  *         parameter as following values:
  *         - I2C_EVENT_CHECK_NONE
  *         - I2C_EVENT_CHECK_ACKFAIL
  *         - I2C_EVENT_CHECK_STOP
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
static i2c_status_type i2c_wait_flag(i2c_handle_type* hi2c, uint32_t flag, uint32_t event_check, uint32_t timeout)
{
	uint32_t TickStart = SystemTimer_GetTick();
  
	if(flag == I2C_BUSYF_FLAG)
  {
    while(i2c_flag_get(hi2c->i2cx, flag) != RESET)
    {
      /* check timeout */
			if ( SystemTimer_GetTick() - TickStart > timeout)
      {
        hi2c->error_code = I2C_ERR_TIMEOUT;
        return I2C_ERR_TIMEOUT;
      }
    }
  }
  else
  {
    while(i2c_flag_get(hi2c->i2cx, flag) == RESET)
    {
      /* check the ack fail flag */
      if(event_check & I2C_EVENT_CHECK_ACKFAIL)
      {
        if(i2c_flag_get(hi2c->i2cx, I2C_ACKFAIL_FLAG) != RESET)
        {
          /* clear ack fail flag */
          i2c_flag_clear(hi2c->i2cx, I2C_ACKFAIL_FLAG);

          hi2c->error_code = I2C_ERR_ACKFAIL;

          return I2C_ERR_ACKFAIL;
        }
      }

      /* check the stop flag */
      if(event_check & I2C_EVENT_CHECK_STOP)
      {
        if(i2c_flag_get(hi2c->i2cx, I2C_STOPF_FLAG) != RESET)
        {
          /* clear stop flag */
          i2c_flag_clear(hi2c->i2cx, I2C_STOPF_FLAG);

          i2c_reset_ctrl2_register(hi2c);

          hi2c->error_code = I2C_ERR_STOP;

          return I2C_ERR_STOP;
        }
      }

      /* check timeout */
			if ( SystemTimer_GetTick() - TickStart > timeout)
      {
        hi2c->error_code = I2C_ERR_TIMEOUT;
        return I2C_ERR_TIMEOUT;
      }
    }
  }

  return I2C_OK;
}

/**
  * @brief  reset ctrl2 register.
  * @param  hi2c: the handle points to the operation information.
  * @retval none.
  */
static void i2c_reset_ctrl2_register(i2c_handle_type* hi2c)
{
  hi2c->i2cx->ctrl2_bit.saddr   = 0;
  hi2c->i2cx->ctrl2_bit.readh10 = 0;
  hi2c->i2cx->ctrl2_bit.cnt     = 0;
  hi2c->i2cx->ctrl2_bit.rlden   = 0;
  hi2c->i2cx->ctrl2_bit.dir     = 0;
}
