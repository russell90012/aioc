#ifndef AIOC_DEFS_H
#define AIOC_DEFS_H


typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;


typedef enum  // aioc_error_t
{
  error_none = 0,
  error_i2c_bus,
  error_spi_bus,
  error_bad_param,
  error_no_reset,
  error_vendor_id_low,
  error_vendor_id_high,
  error_device_type,
  error_register_access
}
aioc_error_t;

typedef  unsigned int  xaioc_error_t;


#endif  // AIOC_DEFS_H
