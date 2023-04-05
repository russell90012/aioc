#ifndef  AIOC_ADC_H
#define AIOC_ADC_H

// MERCURY HEADER GOES HERE
// TBD

/**
 * @file:    aioc_adc.h
 * @author:  Russell Shahenian
 * @since:   4/4/23
 * @brief:   This file contains the public AIOC adc module specifications.
 */

#include "aioc_defs.h"


//================================
// Public specifications.
//================================

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

typedef void* aioc_adc_handle_t;


/**
 * Perform ADC self-check, configuration, and start conversion mode for
 * this ADC.  And return handle to this ADC.
 *
 * @param aioc_adc_id is the ADC that gets initialized.
 *
 * @param aioc_adc_handle is the handle for this ADC that is passed back to
 *        the caller.
 *
 * @return error handling result code.
 */
aioc_error_t aioc_adc_init(
  aioc_adc_id_t aioc_adc_id,
  aioc_adc_handle_t* aioc_adc_handle);

/**
 * Perform a single-cycle mode adc conversion for for the specified input
 * on the ADC specified by the handle.
 *
 * @param adc_handle is handle relevent to the ADC performing the conversion.
 *
 * @param adc_input is the input on the ADC that's getting converted.
 *
 * @return error handling result code.
 */
aioc_error_t aioc_adc_convert_single_cycle(
  aioc_adc_handle_t adc_handle, 
  aioc_adc_input_t adc_input,
  uint16_t* result);


#endif  // AIOC_ADC_H
