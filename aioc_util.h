#ifndef AIOC_UTIL_H
#define AIOC_UTIL_H

#include "aioc_defs.h"


aioc_error_t
aioc_util_delay_ns(uint32_t delay);




aioc_error_t
aioc_util_ultrascale_gpio_pulse_low(
  uint32_t convert_id, 
  uint32_t pulse_duration);




aioc_error_t
aioc_util_spi_open(uint32_t dev_id, uint32_t cs_id)  ;

aioc_error_t
aioc_util_spi_close(void);




aioc_error_t
aioc_util_i2c_open(void);

aioc_error_t
aioc_util_i2c_close(void);

aioc_error_t
aioc_util_spi_transaction(uint8_t* data, uint32_t number_of_bytes);




aioc_error_t aioc_util_i2c_open(void);

aioc_error_t aioc_util_i2c_close(void);

aioc_error_t aioc_util_i2c_write(
  uint32_t dev_adrs,
  uint32_t reg_adrs,
  uint8_t* data,
  uint32_t num_bytes);

#endif  // AIOC_UTIL_H
