// MERCURY HEADER GOES HERE
// TBD

#include "aioc_mux.h"
#include "aioc_i2c_gpio.h"


//================================
// Public  definitions.
//================================

//==============================================================================
//==============================================================================
aioc_error_t
aioc_mux_switch_lines(
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
          if (e)
          {  return e;  }
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK2_A0, 0);
          if (e)
          {  return e;  }
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK1_A1, 0);
          if (e)
          {  return e;  }
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK2_A1, 0);
          if (e)
          {  return e;  }
          break;

        case AIOC_MUX_LINES_BIT_LOW:
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK1_A0, 1);
          if (e)
          {  return e;  }
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK2_A0, 1);
          if (e)
          {  return e;  }
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK1_A1, 0);
          if (e)
          {  return e;  }
          aioc_i2c_gpio_pin_level_set(A5V_SW_BANK2_A1, 0);
          if (e)
          {  return e;  }
          break;

        case AIOC_MUX_LINES_BIT_HIGH:
          e = aioc_i2c_gpio_pin_level_set(A5V_SW_BANK1_A0, 0);
          if (e)
          {  return e;  }
          e = aioc_i2c_gpio_pin_level_set(A5V_SW_BANK2_A0, 0);
          if (e)
          {  return e;  }
          e = aioc_i2c_gpio_pin_level_set(A5V_SW_BANK1_A1, 1);
          if (e)
          {  return e;  }
          e = aioc_i2c_gpio_pin_level_set(A5V_SW_BANK2_A1, 1);
          if (e)
          {  return e;  }
          break;
     }
     break;
  }
 
  return error_none;
}
