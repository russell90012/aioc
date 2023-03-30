#ifndef AIOC_API_H
#define AIOC_API_H

//==============================================================================
//==============================================================================

#include "aioc_defs.h"



//==============================================================================
//==============================================================================

//==============================================================================
//==============================================================================
// Public functions declarations.
//==============================================================================
//==============================================================================

typedef enum
{ 
  // AI Analog 0-5 VDC
  AIOC_AI_LEFT_FWD_OVER_PRESSURE_SENSOR = 0,
  AIOC_AI_LEFT_AFT_OVER_PRESSURE_SENSOR,
  AIOC_AI_RIGHT_FWD_OVER_PRESSURE_SENSOR,
  AIOC_AI_RIGHT_AFT_OVER_PRESSURE_SENSOR,
  AIOC_AI_AFT_RIGHT_FEEDPIPE_PRESSURE_SENSOR,
  AIOC_AI_FWD_LEFT_FEEDPIPE_PRESSURE_SENSOR,
  AIOC_AI_AFT_LEFT_FEEDPIPE_PRESSURE_SENSOR,
  AIOC_AI_FWD_RIGHT_FEEDPIPE_PRESSURE_SENSOR,

  // AI Analog 0-7 VDC
  AIOC_AI_PROBE1_LEFT_FWD_FUEL_QUANTITY,
  AIOC_AI_PROBE1_RIGHT_FWD_FUEL_QUANTITY,
  AIOC_AI_PROBE1_LEFT_AFT_FUEL_QUANTITY,
  AIOC_AI_PROBE1_RIGHT_AFT_FUEL_QUANTITY,
  AIOC_AI_PROBE2_LEFT_FWD_FUEL_QUANTITY,
  AIOC_AI_PROBE2_RIGHT_FWD_FUEL_QUANTIT,
  AIOC_AI_PROBE2_LEFT_AFT_FUEL_QUANTITY,
  AIOC_AI_PROBE2_RIGHT_AFT_FUEL_QUANTITY,

  // AI Analog 95 mVDC
  AIOC_AI_IGB_OIL_FILTER_DELTA_P,
  AIOC_AI_IGB_MANIFOLD_OIL_PRESSURE,
  AIOC_AI_MGB_OIL_FILTER_DELTA_P,
  AIOC_AI_MGB_MANIFOLD_OIL_PRESSURE,
  AIOC_AI_TGB_OIL_FILTER_DELTA_P,
  AIOC_AI_TGB_MANIFOLD_OIL_PRESSURE,
  AIOC_AI_NGB1_OIL_FILTER_DELTA_P,
  AIOC_AI_NGB1_MANIFOLD_OIL_PRESSURE,
  AIOC_AI_NGB3_MANIFOLD_OIL_FILTER_DELTA_P,
  AIOC_AI_NGB3_MANIFOLD_OIL_PRESSURE,

  // AI Analog 350-800Ohms
  AIOC_AI_IGB_MANIFOLD_OIL_FILTER_TEMPERATURE,
  AIOC_AI_MGB_OIL_FILTER_TEMPERATURE,
  AIOC_AI_TGB_MANIFOLD_OIL_TEMPERATURE,
  AIOC_AI_NGB1_OIL_FILTER_TEMPERATURE,
  AIOC_AI_NGB3_OIL_FILTER_TEMPERATURE,
  
  AIOC_AI_NOT_DEFINED
}
aioc_analog_id_t;


/**
 * This procedure will initialize the AIOC.  This includes cconfiguration of
 * the ADC's for single-cycle mode conversion and input MUX's switched to
 * standard (non-bit) inputs.
 */
aioc_error_t
aioc_api_init(void);

aioc_error_t
aioc_api_single_channel_conversion(
  aioc_analog_id_t analog_id,
  uint16_t* result);

#endif // AIOC_API_H
