#ifndef  AIOC_ADC_H
#define AIOC_ADC_H

//==============================================================================
//==============================================================================

#include "aioc_defs.h"


//==============================================================================
//==============================================================================
aioc_error_t 
aioc_adc_self_check(void);

aioc_error_t
aioc_adc_configure_single_cycle_mode(void);

aioc_error_t
aioc_adc_to_conversion_mode(void);

#define CONVERSION_MODE_COMMAND_channel_selection(channel_number) \
          (0x10+(channel_number & 0xF))
#define CONVERSION_MODE_COMMAND_register_configuration_mode  (0xA)
aioc_error_t 
aioc_adc_conversion_mode_command_issue(uint32_t command);

aioc_error_t
aioc_adc_conversion_mode_result_read(uint16_t* result);

aioc_error_t
aioc_adc_reset_all(void);


#endif  // AIOC_ADC_H
