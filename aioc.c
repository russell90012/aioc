// MERCURY HEADER GOES HERE
// TBD

#include "aioc.h"
#include "aioc_adc.h"


//==========================
// Private specifications.
//==========================

/**
 * Map the aioc analog input id to an adc device handle and an adc input.
 *
 * @param analog_id is the analog input that gets mapped to the adc handle
 *        and adc input.
 *
 * @param adc_handle is how the handle is returned to the caller.
 *
 * @param adc_input is how the adc input is returned to the caller.
 *
 * @return error handling result code.
 */
static aioc_error_t map_ai_to_adc_handle_and_input(
        aioc_analog_id_t analog_id,
        struct aioc_adc_dev** dev,
        aioc_adc_input_t* adc_input);
    
//==========================
//==========================
static struct aioc_adc_dev* aioc_adc_dev_5v = 0;

static struct aioc_util_i2c_descriptor* i2c_desc;
//==========================
//==========================


//================================
// Public  definitions.
//================================

//==============================================================================
//==============================================================================
aioc_error_t 
aioc_init(void)
{
  aioc_error_t e = error_none;

  // Initialize the i2c bus descriptor.
  e = aioc_util_i2c_init(&i2c_desc);
  if (e)  {  return e;  }

  // Configure the AIOC i2c GPIO.
  e = aioc_i2c_gpio_configure();
  if (e)  {  return e;  }

  // Fill in parameter structures for 5V ADC.  Then initialize it.
  struct aioc_spi_parm_init spi_parm_init =
  {
    .dev_id = 0,
    .cs_id = 0,
  };
  
 	struct aioc_i2c_gpio_parm_init gpio_parm_init_reset_n = 
  {
    .line = A5V_3V3_ADC_RESET_N,
    .desc = i2c_desc,
  };
  
 	struct aioc_i2c_gpio_parm_init gpio_parm_init_busy = 
  {
    .line = A5V_3V3_ADC_BUSY,
    .desc = i2c_desc,
  };
  
	struct aioc_i2c_gpio_parm_init gpio_parm_init_convert = 
  {
    .line = 0,
    .desc = i2c_desc,
  };
  
  struct aioc_adc_parm_init  adc_parm_init_5v =
  {
    .spi_parm_init =     &spi_parm_init,
    .gpio_parm_init_reset_n = &gpio_parm_init_reset_n,
    .gpio_parm_init_busy = &gpio_parm_init_busy,
    .gpio_parm_init_convert = &gpio_parm_init_convert,
  };
  
  e = aioc_adc_init(&aioc_adc_dev_5v, &adc_parm_init_5v);
  if (e)  {  return e;  }

  return error_none;
}


//==============================================================================
//==============================================================================
aioc_error_t 
aioc_analog_input_conversion(aioc_analog_id_t analog_id, uint16_t* result)
{
  aioc_error_t e = error_none;

  static struct aioc_adc_dev* dev = 0;
  aioc_adc_input_t adc_input = 0;
  
  e = map_ai_to_adc_handle_and_input(analog_id, &dev, &adc_input);
  if (e)  {  return e;  }      

  e = aioc_adc_convert_single_cycle(dev, adc_input, result);
  if (e)  {  return e;  }     

  return error_none;
}


//================================
// Private definitions.
//================================

//==============================================================================
//==============================================================================
static aioc_error_t
map_ai_to_adc_handle_and_input(
    aioc_analog_id_t analog_id,
    struct aioc_adc_dev** device,
    aioc_adc_input_t* adc_input)
{
  aioc_error_t e = error_none;
  aioc_adc_input_t input = 0;
  struct aioc_adc_dev* dev = 0;

  switch(analog_id)
  { 
    // AI Analog 0-5 VDC
    case AIOC_AI_LEFT_FWD_OVER_PRESSURE_SENSOR:
      input = A5V_00_MON;
      dev = aioc_adc_dev_5v;
      break;
    
    case AIOC_AI_LEFT_AFT_OVER_PRESSURE_SENSOR:
      input = A5V_01_MON;
      dev = aioc_adc_dev_5v;
      break;
    
    case AIOC_AI_RIGHT_FWD_OVER_PRESSURE_SENSOR:
      input = A5V_02_MON;
      dev = aioc_adc_dev_5v;
      break;
    
    case AIOC_AI_RIGHT_AFT_OVER_PRESSURE_SENSOR:
      input = A5V_03_MON;
      dev = aioc_adc_dev_5v;
      break;
    
    // NOT DOING THESE FOR NOW
    // OBIGGS_INLET_AIR_TEMP_SENSOR  = A5V_04_MON;
    // OBIGGS_INLET_AIR_PRESSURE_SENSOR  = A5V_05_MON;
    // OBIGGS_NEA_OUTLET_PRESSURE_SENSOR  = A5V_06_MON;

    case AIOC_AI_AFT_RIGHT_FEEDPIPE_PRESSURE_SENSOR:
      input = A5V_07_MON;
      dev = aioc_adc_dev_5v;
      break;
    
    case AIOC_AI_FWD_LEFT_FEEDPIPE_PRESSURE_SENSOR:
      input = A5V_08_MON;
      dev = aioc_adc_dev_5v;
      break;
    
    case AIOC_AI_AFT_LEFT_FEEDPIPE_PRESSURE_SENSOR:
      input = A5V_09_MON;
      dev = aioc_adc_dev_5v;
      break;
    
    case AIOC_AI_FWD_RIGHT_FEEDPIPE_PRESSURE_SENSOR:
      input = A5V_10_MON;
      dev = aioc_adc_dev_5v;
      break;

    // NOT DOING THESE FOR NOW
    //A5V_SPARE_1                         = A5V_11_MON
    //A5V_SPARE_2                       = A5V_12_MON;
    
#if  0
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
#endif
    
    default:
      e = error_ai_mapping;
      return e;
    }
  
  *adc_input = input;
  *device = dev;

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
