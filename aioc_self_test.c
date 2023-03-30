//==============================================================================
//==============================================================================
#include "aioc_api.h"

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



