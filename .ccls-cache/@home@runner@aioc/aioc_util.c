//==============================================================================
//==============================================================================
#include "aioc_util.h"

//==============================================================================
//==============================================================================
aioc_error_t aioc_util_delay_ns(uint32_t delay)
{
  // TBD

  return error_none;
}




//==============================================================================
//==============================================================================
aioc_error_t aioc_util_ultrascale_gpio_pulse_low(
  uint32_t convert_id, 
  uint32_t pulse_duration)
{
  // TBD
  
  return error_none;
}




//==============================================================================
//==============================================================================
typedef struct
{
  uint32_t descriptor;
}
aioc_spi_fp_t;

static volatile aioc_spi_fp_t* aioc_spi_fp = 0;


//==============================================================================
//==============================================================================
aioc_error_t aioc_util_spi_open(uint32_t dev_id, uint32_t cs_id)
{
  if (aioc_spi_fp)
  {
    aioc_util_spi_close();
  }
  
  // TBD
   
  return error_none;
}
  
//==============================================================================
//==============================================================================
aioc_error_t aioc_util_spi_close(void)
{
  // TBD
  
  aioc_spi_fp = 0;
  
  return error_none;
}

//==============================================================================
//==============================================================================
aioc_error_t aioc_util_spi_write(uint8_t* data, uint32_t number_of_bytes)
{
  // TBD
  
  return error_none;
}

//==============================================================================
//==============================================================================
aioc_error_t aioc_util_spi_read(uint8_t* data, uint32_t number_of_bytes)
{
  // TBD
  
  return error_none;
}




//==============================================================================
//==============================================================================
typedef struct
{
  uint32_t descriptor;
}
aioc_i2c_fp_t;

static volatile aioc_i2c_fp_t* aioc_i2c_fp = 0;


//==============================================================================
//==============================================================================
aioc_error_t aioc_util_i2c_open(void)
{
  if (aioc_i2c_fp)
  {
    aioc_util_spi_close();
  }

  // TBD
  
  return error_none;
}

//==============================================================================
//==============================================================================
aioc_error_t aioc_util_i2c_close(void)
{
  // TBD

  aioc_i2c_fp = 0;
  
  return error_none;
}
