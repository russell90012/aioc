#include "aioc_mux.h"
#include "aioc_i2c_gpio.h"

//==============================================================================
//==============================================================================

// ADG5209 Truth Table
// A1  A0  EN  On Switch Pair
// X   X   0   None
// 0   0   1   1              Normal lines
// 0   1   1   2              BIT Low lines
// 1   0   1   3              BIT High lines
// 1   1   1   4
// X is don’t care.


//aioc_mux_banks_lines_table[0][0];

//==============================================================================
//==============================================================================
aioc_error_t aioc_mux_switch_lines(aioc_mux_lines_t aioc_mux_lines,
                                   aioc_mux_banks_t aioc_mux_banks)
{
  aioc_error_t e = error_none;
  i2c_gpio_pin_name_t pin_name = 0;
  uint32_t level = 0;
  
    // TBD
  e = aioc_i2c_gpio_pin_level_set(pin_name,  level);
  if (e)
  {
    
  }
  
  return error_none;
}
