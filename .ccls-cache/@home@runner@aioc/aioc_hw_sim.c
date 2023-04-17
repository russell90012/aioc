#include "aioc_hw_sim.h"

void aioc_hw_sim_i2c_write(
  uint32_t dev_adrs,
  uint32_t reg_adrs,
  uint8_t* data,
  uint32_t data_count)
{
  printf("%s: dev_adrs [0x%08X], reg_adrs [0x%08X], "\
    "data [0x%02X], data_count [%d]\n",
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


void aioc_hw_sim_spi_transaction(uint32_t dev_id, uint32_t cs_id, uint8_t* data, uint32_t data_count)
{
  // Check the size of transaction.
  // If it's two then it's a conversion mode transaction.
  // If it's three or more then it's reg transaction.
  
  static uint32_t reset_flag = 1 << 5;
  static uint8_t scratch_pad = 0x00;
 
  printf("%s : enter: data_count [%d], data [0x%02X] [0x%02X] [0x%02X]\n",
    __FUNCTION__, data_count, data[0], data[1], data[2]);

  if (data_count < 3)
  {
    // It's conversion mode tramsaction.
   
  }
  else
  {
    // Its a register transaction.
    // Check for a read flag (for a read).
    
    uint16_t instruction = data[0] << 8 | data[1];
    uint16_t read_flag = instruction & 0x8000;
    instruction &= ~read_flag;
    
    if (read_flag)
    {
      // It's register READ.
      
      // switch on register address.
      switch (instruction)
      {
        case 0x0014:  // STATUS reg
          // Send back reset flag then clear it.
          data[2] = reset_flag;
          reset_flag = 0;
          break;
        case 0x000C:  // VENDOR_L
          data[2] = 0x56;
          break;
        case 0x000D:  // VENDOR_H
          data[2] = 0x04;
          break;
        case 0x0003:  // DEVICE_TYPE;
          data[2] = 0x07;
          break;
        case 0x000A:
          data[2] = scratch_pad;
          break;
        default:
          break;
       }
    }
    else
    {
      // It's register WRITE.
      
      // switch on register address.
      switch (instruction)
      {
        case 0x000A:
          scratch_pad = 0x00;
          scratch_pad = data[2];
          break;
        default:
          break;
      }
    }
  }
  printf("%s : exit : data_count [%d], data [0x%02X] [0x%02X] [0x%02X]\n",
    __FUNCTION__, data_count, data[0], data[1], data[2]);
}

