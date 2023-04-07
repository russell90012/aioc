#include "aioc_hw_sim.h"

void aioc_hw_sim_i2c_write(
  uint32_t dev_adrs,
  uint32_t reg_adrs,
  uint8_t* data,
  uint32_t data_count)
{
  printf("%s: dev_adrs [0x%08X], reg_adrs [0x%08X], data [0x%02X], data_count [%d]\n",
    __FUNCTION__, dev_adrs, reg_adrs, *data, data_count);
  
  switch (dev_adrs)
  {
    default:
      break;
  }
}



void  aioc_hw_sim_delay_ns(size_t delay)
{
  printf("%s : delay of [%d] nanoSeconds\n", __FUNCTION__, delay);
 
}
