// MERCURY HEADER GOES HERE
// TBD


#include "aioc.h"
#include "aioc_mux.h"

#include <stdio.h>


//==========================
// Private definitions.
//==========================

static const float  A5V_H_BIT_VOLTAGE = 3.333;
static const float  A5V_H_BIT_VOLTAGE_TOLERANCE = A5V_H_BIT_VOLTAGE/10.0;
static const float  ADC_FULL_SCALE_VOLTAGE = 5.0;
static const int    ADC_FULL_SCALE_VALUE = 0xFFFF;
   
static const uint16_t A5V_H_BIT_upper_limit  = 
    (uint16_t)((float)(((A5V_H_BIT_VOLTAGE + A5V_H_BIT_VOLTAGE_TOLERANCE) *
    ((float)ADC_FULL_SCALE_VALUE))/ADC_FULL_SCALE_VOLTAGE));
static const uint16_t A5V_H_BIT_lower_limit  =
    (uint16_t)((float)(((A5V_H_BIT_VOLTAGE - A5V_H_BIT_VOLTAGE_TOLERANCE) *
    ((float)ADC_FULL_SCALE_VALUE))/ADC_FULL_SCALE_VOLTAGE));


typedef struct
{
  aioc_analog_id_t ai_id;
  uint16_t extern_upper_limit;
  uint16_t extern_lower_limit;
  uint16_t bit_h_upper_limit;
  uint16_t bit_h_lower_limit;
  uint16_t bit_l_upper_limit;
  uint16_t bit_l_lower_limit;
  char *   name;
}
aioc_test_table_entry_t;

static aioc_test_table_entry_t aioc_test_table[] =
{
// NOTE: Initial test is one 5V ADC input with BIT HIGH input from AMP.
 {AIOC_AI_LEFT_FWD_OVER_PRESSURE_SENSOR,
    0, 0,  A5V_H_BIT_upper_limit, A5V_H_BIT_lower_limit,  0, 0,
    "AIOC_AI_LEFT_FWD_OVER_PRESSURE_SENSOR"},
#if 0
  {AIOC_AI_LEFT_AFT_OVER_PRESSURE_SENSOR,
    0, 0, A5V_H_BIT_upper_limit, A5V_H_BIT_lower_limit,  0, 0,
    "AIOC_AI_LEFT_AFT_OVER_PRESSURE_SENSOR"},
  {AIOC_AI_RIGHT_FWD_OVER_PRESSURE_SENSOR,
    0, 0,  A5V_H_BIT_upper_limit, A5V_H_BIT_lower_limit,  0, 0,
    "AIOC_AI_RIGHT_FWD_OVER_PRESSURE_SENSOR"},
  {AIOC_AI_RIGHT_AFT_OVER_PRESSURE_SENSOR,
    0, 0,  A5V_H_BIT_upper_limit, A5V_H_BIT_lower_limit,  0, 0,
    "AIOC_AI_RIGHT_AFT_OVER_PRESSURE_SENSOR"},
  {AIOC_AI_AFT_RIGHT_FEEDPIPE_PRESSURE_SENSOR,
    0, 0,  A5V_H_BIT_upper_limit, A5V_H_BIT_lower_limit,  0, 0,
    "AIOC_AI_AFT_RIGHT_FEEDPIPE_PRESSURE_SENSOR"},
  {AIOC_AI_FWD_LEFT_FEEDPIPE_PRESSURE_SENSOR,
    0, 0,  A5V_H_BIT_upper_limit, A5V_H_BIT_lower_limit,  0, 0,
    "AIOC_AI_FWD_LEFT_FEEDPIPE_PRESSURE_SENSOR"},
  {AIOC_AI_AFT_LEFT_FEEDPIPE_PRESSURE_SENSOR,
    0, 0,  A5V_H_BIT_upper_limit, A5V_H_BIT_lower_limit,  0, 0,
    "AIOC_AI_AFT_LEFT_FEEDPIPE_PRESSURE_SENSOR"},
  {AIOC_AI_FWD_RIGHT_FEEDPIPE_PRESSURE_SENSOR,
    0, 0,  A5V_H_BIT_upper_limit, A5V_H_BIT_lower_limit,  0, 0,
    "AIOC_AI_FWD_RIGHT_FEEDPIPE_PRESSURE_SENSOR"}
 #endif
};


//==========================
// Public definitions.
//==========================

//==============================================================================
// NOTE: Initial test is one 5V ADC input with BIT HIGH input from AMP.
//==============================================================================
aioc_error_t aioc_self_test(void)
{
  aioc_error_t e = error_none;
  uint16_t result;

  printf("%s: initializing aioc_api.\n", __FUNCTION__);
  
  // Initialize the board.
  e = aioc_init();
  if (e)  {  return e;  }
  printf("%s: ...done.\n", __FUNCTION__);

  printf("%s: testing the BIT HIGH input selections.\n", __FUNCTION__);
  // Test the BIT HIGH input selections.
  {    
    aioc_test_table_entry_t* test_table = aioc_test_table;
    uint32_t test_table_length =
                sizeof(aioc_test_table)/
                sizeof(*aioc_test_table);

    uint32_t i = 0;
    
    printf("%s: switching in the BIT HIGH inputs.\n", __FUNCTION__);   
    // Multiplex in the BIT High inputs for 5V adc.
    e = aioc_mux_switch_lines(AIOC_MUX_BANKS_5V, AIOC_MUX_LINES_BIT_HIGH);
    if (e)  {  return e;  }
    printf("%s: ...done.\n", __FUNCTION__);
  
    printf("%s: processing the list of analog inputs.\n", __FUNCTION__);
    // Process list of analog inputs and verify the results for each input.
    for (i = 0; i < test_table_length; i++)
    {  
      printf("%s: converting analog input: %s\n",
        __FUNCTION__,
        test_table[i].name);
      e = aioc_analog_input_conversion(test_table[i].ai_id, &result);
      if (e)  {  return e;  }
      printf("%s: ...done.\n", __FUNCTION__);
      
      printf("%s: checking result %x, upper limit %x, lower limit %x\n",
        __FUNCTION__,
        result,
        test_table[i].bit_h_upper_limit,
        test_table[i].bit_h_lower_limit);

      if (result < test_table[i].bit_h_lower_limit ||
          result > test_table[i].bit_h_upper_limit)
      {
        printf("%s: ...result out of range, exiting.\n", __FUNCTION__);
      }
      else
      {
        printf("%s: ...result in range, continuing\n", __FUNCTION__);      
      }
    }

    printf("%s: ...done.\n", __FUNCTION__);
  }  
  printf("%s: ...done.\n", __FUNCTION__);

    return error_none;
}

