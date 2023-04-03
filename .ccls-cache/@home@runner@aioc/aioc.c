//==============================================================================
//==============================================================================
#include "aioc.h"

#include "aioc_i2c_gpio.h"
#include "aioc_util.h"
#include "aioc_adc.h"
//==============================================================================
//==============================================================================



// SPI device id and cs table
typedef struct
{
  uint32_t spi_dev_id;
  uint32_t spi_cs_id;
}
aioc_adc_info_t;

aioc_adc_info_t aioc_adc_info_table[] =
{
  {0, 0}, // AIOC_ADC_ID_5V
  {0, 1}, // AIOC_ADC_ID_7V
  {1, 0}, // AIOC_ADC_ID_95mV
  {1, 1}, // AIOC_ADC_ID_RTD
  {1, 2}  // AIOC_ADC_ID_EP10
};

typedef struct
{
  uint32_t      spi_dev_id;
  uint32_t      spi_cs_id;
  uint32_t      gpio_id;
  uint32_t      adc_input;
}
aioc_ai_info_t;


aioc_ai_info_t  aioc_ai_info_table[] = 
{
                 // AI Analog 0-5 VDC
  {0, 0, 0, 0},  // AIOC_AI_LEFT_FWD_OVER_PRESSURE_SENSOR
  {0, 0, 0, 1},  // AIOC_AI_LEFT_AFT_OVER_PRESSURE_SENSOR
  {0, 0, 0, 2},  // AIOC_AI_RIGHT_FWD_OVER_PRESSURE_SENSOR
  {0, 0, 0, 3},  // AIOC_AI_RIGHT_AFT_OVER_PRESSURE_SENSOR
  {0, 0, 0, 4},  // AIOC_AI_AFT_RIGHT_FEEDPIPE_PRESSURE_SENSOR
  {0, 0, 0, 5},  // AIOC_AI_FWD_LEFT_FEEDPIPE_PRESSURE_SENSOR
  {0, 0, 0, 6},  // AIOC_AI_AFT_LEFT_FEEDPIPE_PRESSURE_SENSOR
  {0, 0, 0, 7},  // AIOC_AI_FWD_RIGHT_FEEDPIPE_PRESSURE_SENSOR

                 // AI Analog 0-7 VDC
  {0, 1, 0, 0},  // AIOC_AI_PROBE1_LEFT_FWD_FUEL_QUANTITY
  {0, 1, 0, 1},  // AIOC_AI_PROBE1_RIGHT_FWD_FUEL_QUANTITY
  {0, 1, 0, 2},  // AIOC_AI_PROBE1_LEFT_AFT_FUEL_QUANTITY
  {0, 1, 0, 3},  // AIOC_AI_PROBE1_RIGHT_AFT_FUEL_QUANTITY
  {0, 1, 0, 4},  // AIOC_AI_PROBE2_LEFT_FWD_FUEL_QUANTITY
  {0, 1, 0, 5},  // AIOC_AI_PROBE2_RIGHT_FWD_FUEL_QUANTIT
  {0, 1, 0, 6},  // AIOC_AI_PROBE2_LEFT_AFT_FUEL_QUANTITY
  {0, 1, 0, 7},  // AIOC_AI_PROBE2_RIGHT_AFT_FUEL_QUANTITY
                 //                                                  
                 // AI Analog 95 mVDC
  {1, 1, 1, 0},  // AIOC_AI_IGB_OIL_FILTER_DELTA_P
  {1, 1, 1, 1},  // AIOC_AI_IGB_MANIFOLD_OIL_PRESSURE
  {1, 1, 1, 2},  // AIOC_AI_MGB_OIL_FILTER_DELTA_P
  {1, 1, 1, 3},  // AIOC_AI_MGB_MANIFOLD_OIL_PRESSURE
  {1, 1, 1, 4},  // AIOC_AI_TGB_OIL_FILTER_DELTA_P
  {1, 1, 1, 5},  // AIOC_AI_TGB_MANIFOLD_OIL_PRESSURE
  {1, 1, 1, 6},  // AIOC_AI_NGB1_OIL_FILTER_DELTA_P
  {1, 1, 1, 7},  // AIOC_AI_NGB1_MANIFOLD_OIL_PRESSURE
  {1, 1, 1, 8},  // AIOC_AI_NGB3_MANIFOLD_OIL_FILTER_DELTA_P
  {1, 1, 1, 9},  // AIOC_AI_NGB3_MANIFOLD_OIL_PRESSURE

                 // AI Analog RTD
  {1, 2, 1, 0},  // AIOC_AI_IGB_MANIFOLD_OIL_FILTER_TEMPERATURE
  {1, 2, 1, 1},  // AIOC_AI_MGB_OIL_FILTER_TEMPERATURE
  {1, 2, 1, 2},  // AIOC_AI_TGB_MANIFOLD_OIL_TEMPERATURE
  {1, 2, 1, 3},  // AIOC_AI_NGB1_OIL_FILTER_TEMPERATURE
  {1, 2, 1, 4}   // AIOC_AI_NGB3_OIL_FILTER_TEMPERATURE
};

//==============================================================================
//==============================================================================
// Private declarations and definitions.
//==============================================================================
//==============================================================================
aioc_error_t aioc_reset_all_adc(void);

aioc_error_t map_ai_to_adc_context_and_input(
              aioc_analog_id_t analog_id,
              aioc_adc_context_t* adc_context,
              aioc_adc_input_t* adc_input);

static aioc_adc_context_t aioc_adc_context_5v = 0;

//==============================================================================
//==============================================================================


//==============================================================================
//==============================================================================
// Public functions.
//==============================================================================
//==============================================================================

//==============================================================================
/**
 * This procedure will initialize the AIOC.  This includes cconfiguration of
 * the ADC's for single-cycle mode conversion and input MUX's switched to
 * standard (non-bit) inputs.
 */
//==============================================================================
aioc_error_t aioc_init(void)
{
  aioc_error_t e = error_none;

  // Configure the AIOC i2c GPIO.
  e = aioc_i2c_gpio_configure();
  if (e)
  {
    return e;
  }

  // Reset the ADCs
  e = aioc_reset_all_adc();
  if (e)
  {
    return e;
  }

  // Initialize an ADC.
  e = aioc_adc_init(AIOC_ADC_ID_5V, &aioc_adc_context_5v);
  if (e)
  {
    return error_adc_init;
  }

  return error_none;
}


//==============================================================================
//==============================================================================
aioc_error_t aioc_analog_input_conversion(
              aioc_analog_id_t analog_id, 
              uint16_t* result)
{
  aioc_error_t e = error_none;

  aioc_adc_context_t adc_context = 0;
  aioc_adc_input_t adc_input = 0;
  
  e = map_ai_to_adc_context_and_input(analog_id, &adc_context, &adc_input);
  if (e)
  {
    return e;
  }      

  e = aioc_adc_convert_single_cycle(adc_context, adc_input);
  if (e)
  {
    return e;
  }      

  return error_none;
}


//==============================================================================
//==============================================================================
// Private functions.
//==============================================================================
//==============================================================================

//==============================================================================
//==============================================================================
aioc_error_t aioc_reset_all_adc(void)
{
  aioc_error_t e = error_none;

  // tRESETL is the minimum amount of time that RESET must be driven low,
  // and tHWR_DELAY is the time that the digital host must wait between a
  // RESET falling edge and starting an SPI frame.
  //
  // RESET Low Time:        tRESETL:    10 ns
  // Hardware Reset Delay:  tHWR_DELAY: 310 µs
  // Software Reset Delay:  tSWR_DELAY: 310 µs
  
  e = aioc_i2c_gpio_pin_pulse_low(A5V_3V3_ADC_RESET_N, 10);
  if (e)
  {
    return e;
  }
  
  e = aioc_i2c_gpio_pin_pulse_low(A7V_3V3_ADC_RESET_N, 10);
  if (e)
  {
    return e;
  }
  
  e = aioc_i2c_gpio_pin_pulse_low(A95mV_3V3_ADC_RESET_N, 10);
  if (e)
  {
    return e;
  }
  
  e = aioc_i2c_gpio_pin_pulse_low(ARTD_3V3_ADC_RESET_N, 10);
  if (e)
  {
    return e;
  }
  
  e = aioc_i2c_gpio_pin_pulse_low(EP10V_3V3_ADC_RESET_N, 10);
  if (e)
  {
    return e;
  }
 
  // Insert delay before any spi frames.
  aioc_util_delay_ns(310000);
  
  return error_none;
}

//==============================================================================
//==============================================================================
aioc_error_t map_ai_to_adc_context_and_input(
              aioc_analog_id_t analog_id,
              aioc_adc_context_t* adc_context,
              aioc_adc_input_t* adc_input)
{
  aioc_error_t e = error_none;
  aioc_adc_input_t input = 0;
  aioc_adc_context_t context = 0;

  switch(analog_id)
  { 
    // AI Analog 0-5 VDC
    case AIOC_AI_LEFT_FWD_OVER_PRESSURE_SENSOR:
      input = A5V_00_MON;
      context = aioc_adc_context_5v;
      break;
    
    case AIOC_AI_LEFT_AFT_OVER_PRESSURE_SENSOR:
      input = A5V_01_MON;
      context = aioc_adc_context_5v;
      break;
    
    case AIOC_AI_RIGHT_FWD_OVER_PRESSURE_SENSOR:
      input = A5V_02_MON;
      context = aioc_adc_context_5v;
      break;
    
    case AIOC_AI_RIGHT_AFT_OVER_PRESSURE_SENSOR:
      input = A5V_03_MON;
      context = aioc_adc_context_5v;
      break;
    
    // NOT DOING THESE FOR NOW
    // OBIGGS_INLET_AIR_TEMP_SENSOR  = A5V_04_MON;
    // OBIGGS_INLET_AIR_PRESSURE_SENSOR  = A5V_05_MON;
    // OBIGGS_NEA_OUTLET_PRESSURE_SENSOR  = A5V_06_MON;

    case AIOC_AI_AFT_RIGHT_FEEDPIPE_PRESSURE_SENSOR:
      input = A5V_07_MON;
      context = aioc_adc_context_5v;
      break;
    
    case AIOC_AI_FWD_LEFT_FEEDPIPE_PRESSURE_SENSOR:
      input = A5V_08_MON;
      context = aioc_adc_context_5v;
      break;
    
    case AIOC_AI_AFT_LEFT_FEEDPIPE_PRESSURE_SENSOR:
      input = A5V_09_MON;
      context = aioc_adc_context_5v;
      break;
    
    case AIOC_AI_FWD_RIGHT_FEEDPIPE_PRESSURE_SENSOR:
      input = A5V_10_MON;
      context = aioc_adc_context_5v;
      break;

    // NOT DOING THESE FOR NOW
    //A5V_SPARE_1                         = A5V_11_MON
    //A5V_SPARE_2                       = A5V_12_MON;
    
    // AI Analog 0-7 VDC
    case AIOC_AI_PROBE1_RIGHT_FWD_FUEL_QUANTITY:
    
    case AIOC_AI_PROBE1_LEFT_AFT_FUEL_QUANTITY:
    
    case AIOC_AI_PROBE1_RIGHT_AFT_FUEL_QUANTITY:
    
    case AIOC_AI_PROBE2_LEFT_FWD_FUEL_QUANTITY:
    
    case AIOC_AI_PROBE2_RIGHT_FWD_FUEL_QUANTIT:
    
    case AIOC_AI_PROBE2_LEFT_AFT_FUEL_QUANTITY:
    
    case AIOC_AI_PROBE2_RIGHT_AFT_FUEL_QUANTITY:
    
    // AI Analog 95 mVDC
    case AIOC_AI_IGB_OIL_FILTER_DELTA_P:
    
    case AIOC_AI_IGB_MANIFOLD_OIL_PRESSURE:
    
    case AIOC_AI_MGB_OIL_FILTER_DELTA_P:
    
    case AIOC_AI_MGB_MANIFOLD_OIL_PRESSURE:
    
    case AIOC_AI_TGB_OIL_FILTER_DELTA_P:
    
    case AIOC_AI_TGB_MANIFOLD_OIL_PRESSURE:
    
    case AIOC_AI_NGB1_OIL_FILTER_DELTA_P:
    
    case AIOC_AI_NGB1_MANIFOLD_OIL_PRESSURE:
    
    case AIOC_AI_NGB3_MANIFOLD_OIL_FILTER_DELTA_P:
    
    case AIOC_AI_NGB3_MANIFOLD_OIL_PRESSURE:
  
    // AI Analog 350-800Ohms
    case AIOC_AI_IGB_MANIFOLD_OIL_FILTER_TEMPERATURE:
    
    case AIOC_AI_MGB_OIL_FILTER_TEMPERATURE:
    
    case AIOC_AI_TGB_MANIFOLD_OIL_TEMPERATURE:
    
    case AIOC_AI_NGB1_OIL_FILTER_TEMPERATURE:
    
    case AIOC_AI_NGB3_OIL_FILTER_TEMPERATURE:
      
    default:
      e = error_ai_mapping;
    }

  return e;
}
  



#if 0
//==============================================================================
//==============================================================================
aioc_error_t aioc_ext10_power_disable(void)
{
  // Setup EXT10 Power enable low.
  
  return error_none;
} 
  
//==============================================================================
//==============================================================================
aioc_error_t aioc_ep10_all_disable(void)
{
  // Setup all EP10 enable lines low.
  
  return error_none;
}  




//==============================================================================
//==============================================================================
aioc_error_t aioc_mux_enable_all(void)
{
  // Setup all of the mux enable lines high.
  
  return error_none;
}


//==============================================================================
//==============================================================================
aioc_error_t aioc_mux_select_input_all(void)
{
  // Setup all mux source address lines low.
  
  return error_none;
}
#endif