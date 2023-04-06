
// MERCURY HEADER GOES HERE
// TBD

#include "AnalogInput.h"
#include "aioc.h"
#include "aioc_defs.h"

//==========================
// Private specifications.
//==========================

/**
 * This function maps the legacy analog id to the AIOC verstion.
 *
 * @param analog_id is the legacy analog id that gets mapped to the
 *        AIOC version.
 *
 * @return an AIOC analog id.
 */
static aioc_analog_id_t map_legacy_ai_to_aioc_ai(GENERIC_ANALOG_IN_TYPE analogID);


//==========================
// Public definitions.
//==========================

//==============================================================================
//==============================================================================
UINT16 SampleOneAnalogInput(GENERIC_ANALOG_IN_TYPE analogID)
{
  // Perform AIOC adc converstion for this analog input.
  uint16_t result = 0;
  aioc_error_t e = error_none;

  // map this analog id to aioc analog id.
  aioc_analog_id_t analog_id = AIOC_AI_NOT_DEFINED;
  analog_id = map_legacy_ai_to_aioc_ai(analogID);
  if (analog_id == AIOC_AI_NOT_DEFINED)
  {
    return 0;
  }
  
  e = aioc_analog_input_conversion(analog_id, &result);
  if (e)
  {
    return 0;
  }

  // Return raw data
  return (result);
}



//==========================
// Private definitions.
//==========================

/**
 * Intermediate enumerations that are used for mapping from the legacy 
 * GENERIC_ANALOG_IN_TYPE ids to the AIOC analog input enumerations.
 */
enum
{
   AI_LEFT_FWD_OVERPRESSURE_SENSOR            = ANALOG_IN_68,  // Z2FV_1
   AI_LEFT_AFT_OVERPRESSURE_SENSOR            = ANALOG_IN_66,  // Z2FV_2
   AI_RIGHT_FWD_OVERPRESSURE_SENSOR           = ANALOG_IN_70,  // Z2FV_3
   AI_RIGHT_AFT_OVERPRESSURE_SENSOR           = ANALOG_IN_65,  // Z2FV_4
   AI_OBIGGS_INLET_AIR_TEMP_SENSOR            = ANALOG_IN_69,  // Z2FV_5
   AI_OBIGGS_INLET_AIR_PRESSURE_SENSOR        = ANALOG_IN_67,  // Z2FV_6

   AI_OBIGGS_NEA_OUTLET_PRESSURE_SENSOR       = ANALOG_IN_72,  // Z2FV_7
   AI_AFT_RIGHT_SEFS_PRESSURE_SENSOR          = ANALOG_IN_76,  // Z2FV_8
   AI_FWD_LEFT_SEFS_PRESSURE_SENSOR           = ANALOG_IN_78,  // Z2FV_10
   AI_AFT_LEFT_SEFS_PRESSURE_SENSOR           = ANALOG_IN_73,  // Z2FV_11
   AI_FWD_RIGHT_SEFS_PRESSURE_SENSOR          = ANALOG_IN_77,  // Z2FV_12

   AI_EXC10_MON_8                             = ANALOG_IN_80,  // EXC10_MON_8
   AI_EXC10_MON_9                             = ANALOG_IN_84,  // EXC10_MON_9
   AI_EXC10_MON_10                            = ANALOG_IN_82,  // EXC10_MON_10
   AI_EXC10_MON_11                            = ANALOG_IN_86,  // EXC10_MON_11

   AI_OBIGGS_OXYGEN_SENSOR                    = ANALOG_IN_96,  // Z2TV_0

   AI_IGB_MANIFOLD_OIL_FILTER_TEMPERATURE     = ANALOG_IN_97,  // RTD0
   AI_MGB_MANIFOLD_OIL_FILTER_TEMPERATURE     = ANALOG_IN_101, // RTD1
   AI_TGB_MANIFOLD_OIL_FILTER_TEMPERATURE     = ANALOG_IN_99,  // RTD2

   AI_LEFT_NGB_OIL_FILTER_TEMPERATURE         = ANALOG_IN_104, // RTD2
   AI_RIGHT_NGB_OIL_FILTER_TEMPERATURE        = ANALOG_IN_108, // RTD4
   AI_VAR_TST0                                = ANALOG_IN_106,
   AI_VAR_TST1                                = ANALOG_IN_110,
   AI_VAR_TST2                                = ANALOG_IN_105,
   AI_VAR_TST3                                = ANALOG_IN_109,
   AI_VAR_TST4                                = ANALOG_IN_107,
   AI_VAR_TST5                                = ANALOG_IN_112,
   AI_VAR_TST6                                = ANALOG_IN_116,
   AI_VAR_TST7                                = ANALOG_IN_114,
   AI_VAR_TST8                                = ANALOG_IN_118,
   AI_VAR_TST9                                = ANALOG_IN_113,

   AI_EXC10_MON_4                             = ANALOG_IN_129, // EXC10_MON_4
   AI_EXC10_MON_5                             = ANALOG_IN_133, // EXC10_MON_5
   AI_EXC10_MON_6                             = ANALOG_IN_131, // EXC10_MON_6

   AI_MFCU_ANALOG_X                           = ANALOG_IN_138, // PM8V_0
   AI_MFCU_ANALOG_Y                           = ANALOG_IN_137, // PM8V_1

   AI_IGB_MANIFOLD_OIL_PRESSURE               = ANALOG_IN_141, // AI_Z2110M_0
   AI_IGB_OIL_FILTER_DELTA_P                  = ANALOG_IN_139, // AI_Z2110M_1
   AI_MGB_MANIFOLD_OIL_PRESSURE               = ANALOG_IN_144, // AI_Z2110M_2
   AI_MGB_OIL_FILTER_DELTA_P                  = ANALOG_IN_148, // AI_Z2110M_3
   AI_TGB_MANIFOLD_OIL_PRESSURE               = ANALOG_IN_146, // AI_Z2110M_4
   AI_TGB_OIL_FILTER_DELTA_P                  = ANALOG_IN_150, // AI_Z2110M_5
   AI_LEFT_NGB_MANIFOLD_OIL_PRESSURE          = ANALOG_IN_145, // AI_Z2110M_6
   AI_LEFT_NGB_OIL_FILTER_DELTA_P             = ANALOG_IN_149, // AI_Z2110M_7
   AI_RIGHT_NGB_MANIFOLD_OIL_PRESSURE         = ANALOG_IN_147, // AI_Z2110M_8
   AI_RIGHT_NGB_OIL_FILTER_DELTA_P            = ANALOG_IN_152, // AI_Z2110M_9

   AI_CTRLR_PROBE1_LEFT_FWD_FUEL_QUANTITY     = ANALOG_IN_200, // Z2SV_ADC0
   AI_CTRLR_PROBE1_RIGHT_FWD_FUEL_QUANTITY,                    // Z2SV_ADC1
   AI_CTRLR_PROBE1_LEFT_AFT_FUEL_QUANTITY,                     // Z2SV_ADC2
   AI_CTRLR_PROBE1_RIGHT_AFT_FUEL_QUANTITY,                    // Z2SV_ADC3

   AI_CTRLR_PROBE2_LEFT_FWD_FUEL_QUANTITY     = ANALOG_IN_209, // Z2SV_ADC8
   AI_CTRLR_PROBE2_RIGHT_FWD_FUEL_QUANTITY,                    // Z2SV_ADC9
   AI_CTRLR_PROBE2_LEFT_AFT_FUEL_QUANTITY,                     // Z2SV_ADC10
   AI_CTRLR_PROBE2_RIGHT_AFT_FUEL_QUANTITY,                    // Z2SV_ADC11
};


//==============================================================================
//==============================================================================
static aioc_analog_id_t
map_legacy_ai_to_aioc_ai(GENERIC_ANALOG_IN_TYPE analogID)
{
  aioc_analog_id_t result = AIOC_AI_NOT_DEFINED;

  switch(analogID)
  {
    // AI Analog 0-5 VDC
    case AI_LEFT_FWD_OVERPRESSURE_SENSOR:
      result = AIOC_AI_LEFT_FWD_OVER_PRESSURE_SENSOR;
      break;

    case AI_LEFT_AFT_OVERPRESSURE_SENSOR:
      result = AIOC_AI_LEFT_AFT_OVER_PRESSURE_SENSOR;
      break;

    case AI_RIGHT_FWD_OVERPRESSURE_SENSOR:
      result = AIOC_AI_RIGHT_FWD_OVER_PRESSURE_SENSOR;
      break;
      
    case AI_RIGHT_AFT_OVERPRESSURE_SENSOR:
      result = AIOC_AI_RIGHT_AFT_OVER_PRESSURE_SENSOR;
      break;
      
    case AI_AFT_RIGHT_SEFS_PRESSURE_SENSOR:
      result = AIOC_AI_AFT_RIGHT_FEEDPIPE_PRESSURE_SENSOR;
      break;

    case AI_FWD_LEFT_SEFS_PRESSURE_SENSOR:
      result = AIOC_AI_FWD_LEFT_FEEDPIPE_PRESSURE_SENSOR;
      break;
      
    case AI_AFT_LEFT_SEFS_PRESSURE_SENSOR:
      result = AIOC_AI_AFT_LEFT_FEEDPIPE_PRESSURE_SENSOR;
      break;
      
    case AI_FWD_RIGHT_SEFS_PRESSURE_SENSOR:
      result = AIOC_AI_FWD_RIGHT_FEEDPIPE_PRESSURE_SENSOR;
      break;
      
    
    // AI Analog 0-7 VDC
    case AI_CTRLR_PROBE1_LEFT_FWD_FUEL_QUANTITY:
      result = AIOC_AI_PROBE1_LEFT_FWD_FUEL_QUANTITY;
      break;
      
    case AI_CTRLR_PROBE1_RIGHT_FWD_FUEL_QUANTITY:
      result = AIOC_AI_PROBE1_RIGHT_FWD_FUEL_QUANTITY;
      break;
      
    case AI_CTRLR_PROBE1_LEFT_AFT_FUEL_QUANTITY:
      result = AIOC_AI_PROBE1_LEFT_AFT_FUEL_QUANTITY;
      break;
      
    case AI_CTRLR_PROBE1_RIGHT_AFT_FUEL_QUANTITY:
      result = AIOC_AI_PROBE1_RIGHT_AFT_FUEL_QUANTITY;
      break;
      
    case AI_CTRLR_PROBE2_LEFT_FWD_FUEL_QUANTITY:
      result = AIOC_AI_PROBE2_LEFT_FWD_FUEL_QUANTITY;
      break;
      
    case AI_CTRLR_PROBE2_RIGHT_FWD_FUEL_QUANTITY:
      result = AIOC_AI_PROBE2_RIGHT_FWD_FUEL_QUANTIT;
      break;
      
    case AI_CTRLR_PROBE2_LEFT_AFT_FUEL_QUANTITY:
      result = AIOC_AI_PROBE2_LEFT_AFT_FUEL_QUANTITY;
      break;
      
    case AI_CTRLR_PROBE2_RIGHT_AFT_FUEL_QUANTITY:
      result = AIOC_AI_PROBE2_RIGHT_AFT_FUEL_QUANTITY;
      break;
      
    
    // AI Analog 95 mVDC
    case AI_IGB_OIL_FILTER_DELTA_P:
      result = AIOC_AI_IGB_OIL_FILTER_DELTA_P;
      break;
	  
    case AI_IGB_MANIFOLD_OIL_PRESSURE:      
      result = AIOC_AI_IGB_MANIFOLD_OIL_PRESSURE;
      break;
      
    case AI_MGB_OIL_FILTER_DELTA_P:
      result = AIOC_AI_MGB_OIL_FILTER_DELTA_P;
      break;
      
    case AI_MGB_MANIFOLD_OIL_PRESSURE:
      result = AIOC_AI_MGB_MANIFOLD_OIL_PRESSURE;
      break;
      
    case AI_TGB_OIL_FILTER_DELTA_P:
      result = AIOC_AI_TGB_OIL_FILTER_DELTA_P;
      break;
      
    case AI_TGB_MANIFOLD_OIL_PRESSURE:
      result = AIOC_AI_TGB_MANIFOLD_OIL_PRESSURE;
      break;
      
    case AI_LEFT_NGB_OIL_FILTER_DELTA_P:
      result = AIOC_AI_NGB1_OIL_FILTER_DELTA_P;
      break;
      
    case AI_LEFT_NGB_MANIFOLD_OIL_PRESSURE:
      result = AIOC_AI_NGB1_MANIFOLD_OIL_PRESSURE;
      break;
      
    case AI_RIGHT_NGB_OIL_FILTER_DELTA_P:
      result = AIOC_AI_NGB3_MANIFOLD_OIL_FILTER_DELTA_P;
      break;
      
    case AI_RIGHT_NGB_MANIFOLD_OIL_PRESSURE:
      result = AIOC_AI_NGB3_MANIFOLD_OIL_PRESSURE;
      break;
      
    
    // AI Analog 350-800Ohms
    case AI_IGB_MANIFOLD_OIL_FILTER_TEMPERATURE:
      result = AIOC_AI_IGB_MANIFOLD_OIL_FILTER_TEMPERATURE;
      break;
      
    case AI_MGB_MANIFOLD_OIL_FILTER_TEMPERATURE:
      result = AIOC_AI_MGB_OIL_FILTER_TEMPERATURE;
      break;
      
    case AI_TGB_MANIFOLD_OIL_FILTER_TEMPERATURE:
      result = AIOC_AI_TGB_MANIFOLD_OIL_TEMPERATURE;
      break;
      
    case AI_LEFT_NGB_OIL_FILTER_TEMPERATURE:
      result = AIOC_AI_NGB1_OIL_FILTER_TEMPERATURE;
      break;
      
    case AI_RIGHT_NGB_OIL_FILTER_TEMPERATURE:
      result = AIOC_AI_NGB3_OIL_FILTER_TEMPERATURE;
      break;
  }
 
  return result;
}

