//==============================================================================
//==============================================================================
#include "aioc_util.h"

#include "aioc_hw_sim.h"

//==============================================================================
//==============================================================================
aioc_error_t aioc_util_delay_ns(uint32_t delay)
{
#ifdef AIOC_HW_SIM
  aioc_hw_sim_delay_ns(delay);
#endif

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
#ifdef  AIOC_DEBUG
printf("%s: convert_id [%d], duration [%d]\n", __FUNCTION__,
  convert_id, pulse_duration);
#endif
  
  return error_none;
}




//==============================================================================
//==============================================================================
aioc_error_t 
aioc_util_spi_init(
      struct aioc_util_spi_descriptor ** spi_desc,
      struct aioc_spi_parm_init* parm)
{
    
  // TBD
   
  return error_none;
}
  
//==============================================================================
//==============================================================================
aioc_error_t aioc_util_spi_close(struct aioc_util_spi_descriptor * spi_desc)
{
  // TBD
  
  
  return error_none;
}

//==============================================================================
//==============================================================================
aioc_error_t aioc_util_spi_transaction(
  struct aioc_util_spi_descriptor * spi_desc,
  uint8_t* data,
  uint32_t number_of_bytes)
{
#ifdef AIOC_HW_SIM
  aioc_hw_sim_spi_transaction(data, number_of_bytes);
#endif
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

//==============================================================================
//==============================================================================
aioc_error_t aioc_util_i2c_write(
  uint32_t dev_adrs,
  uint32_t reg_adrs,
  uint8_t* data,
  uint32_t num_bytes)
{
#ifdef AIOC_HW_SIM
  aioc_hw_sim_i2c_write(dev_adrs, reg_adrs, data, num_bytes);
#endif
  
  return error_none;
}
