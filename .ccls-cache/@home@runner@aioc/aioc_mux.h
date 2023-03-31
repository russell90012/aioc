#ifndef AIOC_MUX_H
#define AIOC_MUX_H

#include "aioc_defs.h"

//==============================================================================
//==============================================================================
typedef enum
{
  AIOC_MUX_BANKS_5V,
}
aioc_mux_banks_t;

typedef enum
{
  AIOC_MUX_LINES_EXTERNAL = 0,
  AIOC_MUX_LINES_BIT_LOW,
  AIOC_MUX_LINES_BIT_HIGH,
}
aioc_mux_lines_t;

//==============================================================================
//==============================================================================
aioc_error_t
aioc_mux_switch_lines(aioc_mux_lines_t aioc_mux_lines,
                      aioc_mux_banks_t aioc_mux_banks);

#endif  // AIOC_MUX_H
