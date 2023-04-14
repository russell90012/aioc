#ifndef AIOC_UTIL_H
#define AIOC_UTIL_H

#include "aioc_defs.h"

//
//
//

aioc_error_t
aioc_util_delay_ns(uint32_t delay);




aioc_error_t
aioc_util_ultrascale_gpio_pulse_low(
  uint32_t convert_id, 
  uint32_t pulse_duration);


//
// SPI
//
struct aioc_spi_parm_init
{
  uint32_t dev_id;
  uint32_t cs_id;
};
  


struct aioc_util_spi_descriptor
{
  uint32_t fd;  // TBD
};

aioc_error_t aioc_util_spi_init(
              struct aioc_util_spi_descriptor ** spi_desc,
              struct aioc_spi_parm_init* parm);

aioc_error_t aioc_util_spi_close(struct aioc_util_spi_descriptor * spi_desc);

aioc_error_t aioc_util_spi_transaction(
              struct aioc_util_spi_descriptor * spi_desc,
              uint8_t* data,
              uint32_t number_of_bytes);

//
// I2C
//
struct aioc_util_i2c_descriptor
{
  uint32_t fd;  // TBD
};

aioc_error_t aioc_util_i2c_init(struct aioc_util_i2c_descriptor ** i2c_desc);

aioc_error_t aioc_util_i2c_write(
  uint32_t dev_adrs,
  uint32_t reg_adrs,
  uint8_t* data,
  uint32_t num_bytes);




#endif  // AIOC_UTIL_H
