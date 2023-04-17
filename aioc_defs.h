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
  error_adc_self_check,
  error_adc_init,
  error_ai_mapping,
  error_bad_state,
  error_alloc,
}
aioc_error_t;


#define BOOL(x) (!(!(x)))

#define BIT_SET(arg,posn) ((arg) | (1L << (posn)))
#define BIT_CLR(arg,posn) ((arg) & ~(1L << (posn)))
#define BIT_TST(arg,posn) BOOL((arg) & (1L << (posn)))
#define BIT_FLP(arg,posn) ((arg) ^ (1L << (posn)))


#define AIOC_DEBUG
#endif  // AIOC_DEFS_H
