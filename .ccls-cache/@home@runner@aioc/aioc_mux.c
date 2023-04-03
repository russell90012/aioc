#include "aioc_mux.h"
#include "aioc_i2c_gpio.h"

//==============================================================================
//==============================================================================

// ADG5209 Truth Table
// A1  A0  EN  On Switch Pair
// X   X   0   None
// 0   0   1   1              External lines
// 0   1   1   2              BIT Low lines
// 1   0   1   3              BIT High lines
// 1   1   1   4              NA
// X is don’t care.

//==============================================================================
//==============================================================================
aioc_error_t aioc_mux_switch_lines(
              aioc_mux_banks_t aioc_mux_banks,
              aioc_mux_lines_t aioc_mux_lines)
{
  aioc_error_t e = error_none;
  
  switch (aioc_mux_banks)
  {
    case AIOC_MUX_BANKS_5V:
      switch (aioc_mux_lines)
      {
        case AIOC_MUX_LINES_EXTERNAL:
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK1_A0, 0);
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK2_A0, 0);
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK1_A1, 0);
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK2_A1, 0);
          break;
        case AIOC_MUX_LINES_BIT_LOW:
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK1_A0, 1);
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK2_A0, 1);
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK1_A1, 0);
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK2_A1, 0);
          break;
        case AIOC_MUX_LINES_BIT_HIGH:
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK1_A0, 0);
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK2_A0, 0);
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK1_A1, 1);
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK2_A1, 1);
          break;
     }
     break;
  }
 
  return error_none;
}
