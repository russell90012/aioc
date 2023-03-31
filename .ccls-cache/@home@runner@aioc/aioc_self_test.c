//==============================================================================
//==============================================================================
#include "aioc_api.h"

#include "aioc_mux.h"


//==============================================================================
//==============================================================================
typedef struct
{
  aioc_analog_id_t ai_id;
  uint16_t upper_limit;
  uint16_t lower_limit;
}
aioc_test_table_entry_t;

static aioc_test_table_entry_t bit_high_test_table[] =
// TBD
{
  {AIOC_AI_LEFT_FWD_OVER_PRESSURE_SENSOR, 0, 0},
  {AIOC_AI_LEFT_AFT_OVER_PRESSURE_SENSOR, 0, 0},
  {AIOC_AI_RIGHT_FWD_OVER_PRESSURE_SENSOR, 0, 0},
  {AIOC_AI_RIGHT_AFT_OVER_PRESSURE_SENSOR, 0, 0},
  {AIOC_AI_AFT_RIGHT_FEEDPIPE_PRESSURE_SENSOR, 0, 0},
  {AIOC_AI_FWD_LEFT_FEEDPIPE_PRESSURE_SENSOR, 0, 0},
  {AIOC_AI_AFT_LEFT_FEEDPIPE_PRESSURE_SENSOR, 0, 0},
  {AIOC_AI_FWD_RIGHT_FEEDPIPE_PRESSURE_SENSOR, 0, 0}
};

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

  // Test the BIT HIGH input selections.
  {    
    aioc_test_table_entry_t* test_table = bit_high_test_table;
    uint32_t test_table_length =
                sizeof(bit_high_test_table)/
                sizeof(*bit_high_test_table);
    uint32_t i = 0;
    
    // Multiplex in the BIT High inputs for 5V adc.
    e = aioc_mux_switch_lines(AIOC_MUX_LINES_BIT_HIGH, AIOC_MUX_BANKS_5V);
    if (e)
    {
      return e;
    }
  
    // Process list of analog inputs and verify the results for each input.
    for (i = 0; i < test_table_length; i++)
    {
      aioc_analog_id_t ai_id = test_table[i].ai_id;
      uint16_t upper_limit = test_table[i].upper_limit;
      uint16_t lower_limit = test_table[i].lower_limit;
      
      e = aioc_api_single_channel_conversion(ai_id, &result);
      if (e)
      {
        return e;
      }
    }

    //Check the results.
    // TBD
  }  
  return error_none;
}

