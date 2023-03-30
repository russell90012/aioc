/*******************************************************************************
 *
 *  File Name   :  AnalogInput.c
 *
 *  Version     :  SVN $Revision:: 519    $
 *
 *  Description :  This module provides functions to initialize the
 *                 internal and external ADCs and retrieve data.
 *
 *  Author      :  Brefeld, Tony
 *
 *  Note        :  Curtiss-Wright Controls, Inc. - All right reserved
 *
 *  Note2       :  Used the following reference documents:
 *                 DOORS EXPORT 104-0091 SRD 08-16-10 (PER SAC REV4)
 *                 105-0019 ICD vss48
 *                 121-0648 DCU Hardware Address Mapping vss13
 *                 104-0093 OFP SDD dated: 7/8/2010
 *
 *
 *  Date      PCR#  User         Change Description
 *  --------  ----  -----------  -----------------------------------------------
 *  09/07/10  N/A   T. Brefeld   Initial
 *  8/21/12  1446   J. Squiers   Corrected spacing to match standards
 *
 ******************************************************************************/
#if	1
/*******************************************************************************
 *                             # I n c l u d e s                               *
 ******************************************************************************/
#include "AnalogInput.h"

#include "aioc_defs.h"

#include "aioc_api.h"

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


/*******************************************************************************
 *             I n t e r n a l   T y p e   D e c l a r a t i o n s             *
 ******************************************************************************/

/* none */

/*******************************************************************************
 *             I n t e r n a l   D a t a   D e c l a r a t i o n s             *
 ******************************************************************************/

/* none */

/*******************************************************************************
 *           I n t e r n a l   F u n c t i o n   P r o t o t y p e s           *
 ******************************************************************************/

/* none */

/*******************************************************************************
 *                   F u n c t i o n   D e f i n i t i o n s                   *
 ******************************************************************************/

/*******************************************************************************
 *
 * Function    :  map_to_aioc_analog_id
 *
 * Description :  This function maps the legacy analog id to the aioc verstion.
 *
 * Parameter   :  UINT32 analogID
 *
 * Returns     :  UINT32 analog_id is the AIOC version.
 *
 ******************************************************************************/
static aioc_analog_id_t map_to_aioc_analog_id(GENERIC_ANALOG_IN_TYPE analogID)
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


//==============================================================================
//==============================================================================
UINT16 SampleOneAnalogInput(GENERIC_ANALOG_IN_TYPE analogID)
{
  // Perform AIOC adc converstion for this analog input.
  uint16_t result = 0;
  aioc_error_t e = error_none;

  // map this analog id to aioc analog id.
  aioc_analog_id_t analog_id = AIOC_AI_NOT_DEFINED;
  analog_id = map_to_aioc_analog_id(analogID);
  if (analog_id == AIOC_AI_NOT_DEFINED)
  {
    return 0;
  }
  
  e = aioc_api_single_channel_conversion(analog_id, &result);
  if (e)
  {
    return 0;
  }

  // Return raw data
  return (result);
}

#endif
