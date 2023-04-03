#ifndef  AIOC_ADC_H
#define AIOC_ADC_H

//==============================================================================
//==============================================================================

#include "aioc_defs.h"

typedef enum
{
  AIOC_ADC_ID_5V,
  AIOC_ADC_ID_7V,
  AIOC_ADC_ID_95mV,
  AIOC_ADC_ID_RTD,
  AIOC_ADC_ID_EP10,
  
  NUMBER_OF_AIOC_ADC_IDS
}
aioc_adc_id_t;

typedef void* aioc_adc_handle_t;


typedef enum
{
  // AI Analog 0-5 VDC
  A5V_00_MON = 0,
  A5V_01_MON,
  A5V_02_MON,
  A5V_03_MON,
  A5V_04_MON,
  A5V_05_MON,
  A5V_06_MON,
  A5V_07_MON,
  A5V_08_MON,
  A5V_09_MON,
  A5V_10_MON,
  A5V_11_MON,
  A5V_12_MON
}
aioc_adc_input_t;

//==============================================================================
//==============================================================================
// Public functions declarations.
//==============================================================================
//==============================================================================
aioc_error_t
aioc_adc_init(aioc_adc_id_t aioc_adc_id, aioc_adc_handle_t* aioc_adc_handle);

aioc_error_t
aioc_adc_convert_single_cycle(
  aioc_adc_handle_t adc_handle, 
  aioc_adc_input_t adc_input,
  uint16_t* result);




#endif  // AIOC_ADC_H
