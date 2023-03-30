#ifndef  AIOC_ADC_H
#define AIOC_ADC_H

//==============================================================================
//==============================================================================

#include "aioc_defs.h"


//==============================================================================
//==============================================================================
// Public functions declarations.
//==============================================================================
//==============================================================================

aioc_error_t
aioc_adc_self_check(void);

aioc_error_t
aioc_adc_configure_single_cycle_mode(void);

// Issue specific input channel selection command.
aioc_error_t
aioc_adc_conversion_mode_command_channel_selection(uint32_t input);

// Read 16 bit result from ADC.
// NOTE: Must be in conversion mode for this to work.
aioc_error_t
aioc_adc_conversion_mode_result_read(uint16_t* result);

aioc_error_t
aioc_adc_to_conversion_mode(void);

aioc_error_t
aioc_adc_to_register_mode(void);



#endif  // AIOC_ADC_H
