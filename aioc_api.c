//==============================================================================
//==============================================================================
#include "aioc_api.h"

#include "aioc_i2c_gpio.h"
#include "aioc_util.h"
#include "aioc_adc.h"
//==============================================================================
//==============================================================================

typedef enum
{
  AIOC_ADC_ID_5V,
  AIOC_ADC_ID_7V,
  AIOC_ADC_ID_95mV,
  AIOC_ADC_ID_RTD,
  AIOC_ADC_ID_EP10,
  
  NUMBER_OF_AIOC_ADC_ID
}
aioc_adc_id_t;


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
aioc_error_t aioc_api_adc_reset_all(void);

//==============================================================================
//==============================================================================
aioc_error_t aioc_self_test(void)
{
  aioc_error_t e = error_none;
  uint16_t result;
 
  // Initialize the board devices.
  e = aioc_api_init();
  if (e)
  {
    return e;
  }
  
  // Stimulate the signal under test.
  // TBD
  
  e = aioc_api_single_channel_conversion(
        AIOC_AI_LEFT_FWD_OVER_PRESSURE_SENSOR,
        &result);
  if (e)
  {
    return e;
  }
  
  //Check the results.
  // TBD
  
  return error_none;
}




//==============================================================================
/**
 * This procedure will initialize the AIOC.  This includes cconfiguration of
 * the ADC's for single-cycle mode conversion and input MUX's switched to
 * standard (non-bit) inputs.
 */
//==============================================================================
aioc_error_t aioc_api_init(void)
{
  aioc_error_t e = error_none;
  
  // Configure the AIOC i2c GPIO.
  e = aioc_i2c_gpio_configure();
  if (e)
  {
    return e;
  }

  // Reset the ADCs
  e = aioc_api_adc_reset_all();
  if (e)
  {
    return e;
  }
  
  // Perform ADC self-check, configuration, and start conversion mode for all five ADCs.
  // NOTE: just do 5V for now.
  e = aioc_util_spi_open(
        aioc_adc_info_table[AIOC_ADC_ID_5V].spi_dev_id,
        aioc_adc_info_table[AIOC_ADC_ID_5V].spi_cs_id);
  if (e)
  {
    return e;
  }      
  
  // Perform ADC self-check.
  e = aioc_adc_self_check();  
  if (e)
  {
    return e;
  }
 
  // Configure the ADC
  e = aioc_adc_configure_single_cycle_mode();
  if (e)
  {
    return e;
  }
  
  // Start conversion mode.
  e = aioc_adc_to_conversion_mode();
  if (e)
  {
    return e;
  }      

  e = aioc_util_spi_close();
  if (e)
  {
    return e;
  }      
 
  return error_none;
}

//==============================================================================
//==============================================================================
aioc_error_t aioc_api_single_channel_conversion(
              aioc_analog_id_t analog_id, 
              uint16_t* result)
{
  aioc_error_t e = error_none;
  uint32_t convert_id =aioc_ai_info_table[analog_id].gpio_id;
  uint32_t input = aioc_ai_info_table[analog_id].adc_input;
 
  e = aioc_util_spi_open(
        aioc_ai_info_table[analog_id].spi_dev_id,
        aioc_ai_info_table[analog_id].spi_cs_id);
  if (e)
  {
    return e;
  }      

  // Issue specific input channel selection command.
  e = aioc_adc_conversion_mode_command_issue(
        CONVERSION_MODE_COMMAND_channel_selection(input));
  if (e)
  {
    return e;
  }
  
  // Pulse the appropriate ADC convert signal active (low) for appropriate
  // duration.
  //  CNV Low Time: tCNVL: 80 ns
  e = aioc_util_ultrascale_gpio_pulse_low(convert_id, 80);
  if (e)
  {
    return e;
  }

  // Delay for conversion time.
  //  Conversion Time: tCONVERT: 380-415 ns.
  aioc_util_delay_ns(500);
  
  // Read the conversion result.
  e = aioc_adc_conversion_mode_result_read(result);
  if (e)
  {
    return e;
  }
  
  e = aioc_util_spi_close();
  if (e)
  {
    return e;
  }
    
  // return conversion.
  return error_none;
}

//==============================================================================
//==============================================================================
aioc_error_t aioc_api_adc_reset_all(void)
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
aioc_error_t aioc_adc_conversion_single_cycle_mode(
  uint32_t convert_id,
  uint32_t input,
  uint16_t* result)
{
  aioc_error_t e = error_none;

  // Issue specific input channel selection command.
  e = aioc_adc_conversion_mode_command_issue(
        CONVERSION_MODE_COMMAND_channel_selection(input));
  if (e)
  {
    return e;
  }
  
  // Pulse the appropriate ADC convert signal active (low) for appropriate
  // duration.
  //  CNV Low Time: tCNVL: 80 ns
  e = aioc_util_ultrascale_gpio_pulse_low(convert_id, 80);
  if (e)
  {
    return e;
  }

  // Delay for conversion time.
  //  Conversion Time: tCONVERT: 380-415 ns.
  aioc_util_delay_ns(500);
  
  // Read the conversion result.
  e = aioc_adc_conversion_mode_result_read(result);
  if (e)
  {
    return e;
  }
  
  return error_none;
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
