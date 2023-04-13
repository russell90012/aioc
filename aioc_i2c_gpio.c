// MERCURY HEADER GOES HERE
// TBD

#include "aioc_i2c_gpio.h"
#include "aioc_util.h"

#include <string.h>


//==========================
// Private specifications.
//==========================

// The table below is based on the confugration table in the specification
// file (the header).
typedef struct
{
  uint32_t device_adrs;
  uint32_t device;
  uint32_t bank;
  uint32_t pin;
  uint32_t configuration;
  uint32_t polarity;
  uint32_t initial_value;
}
i2c_gpio_pin_conf_t;
    
i2c_gpio_pin_conf_t i2c_gpio_pin_configuration_table[] =
{
  {0X20, 0, 0, 0, 0, 0, 0},   // A5V_SW_BANK1_A0,
  {0X20, 0, 0, 1, 0, 0, 0},   // A5V_SW_BANK2_A0,
  {0X20, 0, 0, 2, 0, 0, 0},   // A5V_SW_BANK1_A1,
  {0X20, 0, 0, 3, 0, 0, 0},   // A5V_SW_BANK2_A1,
  {0X20, 0, 0, 4, 0, 0, 1},   // A5V_SW_BANK1_EN,
  {0X20, 0, 0, 5, 0, 0, 1},   // A5V_SW_BANK2_EN,
  {0X20, 0, 0, 6, 0, 0, 1},   // A5V_3V3_ADC_RESET_N,
  {0X20, 0, 0, 7, 1, 0, 0},   // A5V_3V3_ADC_BUSY,

  {0X20, 0, 1, 0, 0, 0, 0},   // A7V_SW_BANK1_A0,
  {0X20, 0, 1, 1, 0, 0, 0},   // A7V_SW_BANK2_A0,
  {0X20, 0, 1, 2, 0, 0, 0},   // A7V_SW_BANK1_A1,
  {0X20, 0, 1, 3, 0, 0, 0},   // A7V_SW_BANK2_A1,
  {0X20, 0, 1, 4, 0, 0, 1},   // A7V_SW_BANK1_EN,
  {0X20, 0, 1, 5, 0, 0, 1},   // A7V_SW_BANK2_EN,
  {0X20, 0, 1, 6, 0, 0, 1},   // A7V_3V3_ADC_RESET_N,
  {0X20, 0, 1, 7, 1, 0, 0},   // A7V_3V3_ADC_BUSY,


  {0X21, 1, 0, 0, 0, 0, 0},   // A95mV_SW_BANK1_A0,
  {0X21, 1, 0, 1, 0, 0, 0},   // A95mV_SW_BANK2_A0,
  {0X21, 1, 0, 2, 0, 0, 0},   // A95mV_SW_BANK1_A1,
  {0X21, 1, 0, 3, 0, 0, 0},   // A95mV_SW_BANK2_A1,
  {0X21, 1, 0, 4, 0, 0, 1},   // A95mV_SW_BANK1_EN,
  {0X21, 1, 0, 5, 0, 0, 1},   // A95mV_SW_BANK2_EN,
  {0X21, 1, 0, 6, 0, 0, 1},   // A95mV_3V3_ADC_RESET_N,
  {0X21, 1, 0, 7, 1, 0, 0},   // A95mV_3V3_ADC_BUSY,

  {0X21, 1, 1, 0, 0, 0, 0},   // ARTD_SW_A0,
  {0X21, 1, 1, 1, 0, 0, 0},   // ARTD_SW_A1,
  {0X21, 1, 1, 2, 0, 0, 1},   // ARTD_SW_EN,
  {0X21, 1, 1, 3, 0, 0, 1},   // ARTD_3V3_ADC_RESET_N,
  {0X21, 1, 1, 4, 1, 0, 0},   // ARTD_3V3_ADC_BUSY,
  {0X21, 1, 1, 5, 0, 0, 1},   // EP10V_3V3_ADC_RESET_N,
  {0X21, 1, 1, 6, 1, 0, 0},   // EP10V_3V3_ADC_BUSY,
  {0X21, 1, 1, 7, 1, 0, 0},   // NC_0, 


  {0X22, 2, 0, 0, 0, 0, 0},   // A10V_SW_A0,
  {0X22, 2, 0, 1, 0, 0, 0},   // A10V_SW_A1,
  {0X22, 2, 0, 2, 0, 0, 1},   // A10V_SW_EN,
  {0X22, 2, 0, 3, 1, 0, 0},   // A5V_ADC_PG,
  {0X22, 2, 0, 4, 1, 0, 0},   // A7V_ADC_PG,
  {0X22, 2, 0, 5, 1, 0, 0},   // A95mV_ADC_PG,
  {0X22, 2, 0, 6, 1, 0, 0},   // ARTD_ADC_PG,
  {0X22, 2, 0, 7, 1, 0, 0},   // EP10V_ADC_PG,

  {0X22, 2, 1, 0, 0, 0, 0},   // AZ_EL_SW_A0,
  {0X22, 2, 1, 1, 0, 0, 0},   // AZ_EL_SW_A1,
  {0X22, 2, 1, 2, 0, 0, 1},   // AZ_EL_SW_EN,
  {0X22, 2, 1, 3, 0, 0, 1},   // RTD_EXC_00_OFF_ON_N,
  {0X22, 2, 1, 4, 0, 0, 1},   // RTD_EXC_01_OFF_ON_N,
  {0X22, 2, 1, 5, 0, 0, 1},   // RTD_EXC_02_OFF_ON_N,
  {0X22, 2, 1, 6, 0, 0, 1},   // RTD_EXC_03_OFF_ON_N,
  {0X22, 2, 1, 7, 0, 0, 1},   // RTD_EXC_04_OFF_ON_N,



  {0x23, 3, 0, 0, 0, 0, 0},   // EP10V_00_EN,
  {0x23, 3, 0, 1, 0, 0, 0},   // EP10V_01_EN,
  {0x23, 3, 0, 2, 0, 0, 0},   // EP10V_02_EN,
  {0x23, 3, 0, 3, 0, 0, 0},   // EP10V_03_EN,
  {0x23, 3, 0, 4, 0, 0, 0},   // EP10V_04_EN,
  {0x23, 3, 0, 5, 0, 0, 0},   // EP10V_05_EN,
  {0x23, 3, 0, 6, 0, 0, 0},   // EP10V_06_EN,
  {0x23, 3, 0, 7, 0, 0, 0},   // EP10V_07_EN,
                           // 
  {0x23, 3, 1, 0, 0, 0, 0},   // EP10V_08_EN,
  {0x23, 3, 1, 1, 0, 0, 0},   // EP10V_09_EN,
  {0x23, 3, 1, 2, 0, 0, 0},   // EP10V_10_EN,
  {0x23, 3, 1, 3, 0, 0, 0},   // EP10V_11_EN,
  {0x23, 3, 1, 4, 0, 0, 0},   // P10V_POWER_EN,  
  {0x23, 3, 1, 5, 1, 0, 0},   // NC_1,      
  {0x23, 3, 1, 6, 1, 0, 0},   // NC_2,      
  {0x23, 3, 1, 7, 1, 0, 0},   // NC_3,      


  {0X24, 4, 0, 0, 1, 0, 0},   // EP10V_00_PG_N,
  {0X24, 4, 0, 1, 1, 0, 0},   // EP10V_01_PG_N,
  {0X24, 4, 0, 2, 1, 0, 0},   // EP10V_02_PG_N,
  {0X24, 4, 0, 3, 1, 0, 0},   // EP10V_03_PG_N,
  {0X24, 4, 0, 4, 1, 0, 0},   // EP10V_04_PG_N,
  {0X24, 4, 0, 5, 1, 0, 0},   // EP10V_05_PG_N,
  {0X24, 4, 0, 6, 1, 0, 0},   // EP10V_06_PG_N,
  {0X24, 4, 0, 7, 1, 0, 0},   // EP10V_07_PG_N,
                           // 
  {0X24, 4, 1, 0, 1, 0, 0},   // EP10V_08_PG_N,
  {0X24, 4, 1, 1, 1, 0, 0},   // EP10V_09_PG_N,
  {0X24, 4, 1, 2, 1, 0, 0},   // EP10V_10_PG_N,
  {0X24, 4, 1, 3, 1, 0, 0},   // EP10V_11_PG_N,
  {0X24, 4, 1, 4, 1, 0, 0},   // P10V_PG_0,     
  {0X24, 4, 1, 5, 1, 0, 0},   // P10V_PG_1,     
  {0X24, 4, 1, 6, 1, 0, 0},   // NC_4,         
  {0X24, 4, 1, 7, 1, 0, 0},   // NC_5,         


  {0X25, 5, 0, 0, 1, 0, 0},   // EP10V_00_FLG_N,
  {0X25, 5, 0, 1, 1, 0, 0},   // EP10V_01_FLG_N,
  {0X25, 5, 0, 2, 1, 0, 0},   // EP10V_02_FLG_N,
  {0X25, 5, 0, 3, 1, 0, 0},   // EP10V_03_FLG_N,
  {0X25, 5, 0, 4, 1, 0, 0},   // EP10V_04_FLG_N,
  {0X25, 5, 0, 5, 1, 0, 0},   // EP10V_05_FLG_N,
  {0X25, 5, 0, 6, 1, 0, 0},   // EP10V_06_FLG_N,
  {0X25, 5, 0, 7, 1, 0, 0},   // EP10V_07_FLG_N,

  {0X25, 5, 1, 0, 1, 0, 0},   // EP10V_08_FLG_N,
  {0X25, 5, 1, 1, 1, 0, 0},   // EP10V_09_FLG_N,
  {0X25, 5, 1, 2, 1, 0, 0},   // EP10V_10_FLG_N,
  {0X25, 5, 1, 3, 1, 0, 0},   // EP10V_11_FLG_N,
  {0X25, 5, 1, 4, 1, 0, 0},   // NC_6,       
  {0X25, 5, 1, 5, 1, 0, 0},   // NC_7,       
  {0X25, 5, 1, 6, 1, 0, 0},   // NC_8,       
  {0X25, 5, 1, 7, 1, 0, 0},   // NC_9,       
};

// The i2c GPIO register map (along with addresses).
/**********
Control Register and Command Byte

CONTROL REGISTER BITS   COMMAND
B2  B1  B0              BYTE    REGISTER        PROTOCOL          POWER-UP
0   0   0               0x00    Input Bank 0    Read byte         xxxx xxxx
0   0   1               0x01    Input Bank 1    Read byte         xxxx xxxx
0   1   0               0x02    Output Bank 0   Read-write byte   1111 1111
0   1   1               0x03    Output Bank 1   Read-write byte   1111 1111
1   0   0               0x04    Polarity 
                                Inversion 
                                Bank 0          Read-write byte   0000 0000
1   0   1               0x05    Polarity
                                Inversion 
                                Bank 1          Read-write byte   0000 0000
1   1   0               0x06    Configuration 
                                Bank 0          Read-write byte   1111 1111
1   1   1               0x07    Configuration
                                Bank 1          Read-write byte   1111 1111
**********/
#define NUM_BANKS         2
#define NUM_DEVICES       6
#define DEVICE_ADRS_BASE  0x20

typedef struct
{
  uint8_t input[NUM_BANKS];
  uint8_t output[NUM_BANKS];
  uint8_t polarity[NUM_BANKS];
  uint8_t configuration[NUM_BANKS];
}
register_map_t;

// Table of i2c GPIO register map structures.  One register map per device.

typedef struct
{
  register_map_t  regs;
  uint32_t        address;
  }
i2c_gpio_device_t;

i2c_gpio_device_t i2c_gpio_devices[NUM_DEVICES] = {0};

// Commands used to write to registers (basically addresses).
enum
{
  COMMAND_BYTE_OUTPUT_0 = 2,
  COMMAND_BYTE_OUTPUT_1,
  COMMAND_BYTE_POLARITY_0,
  COMMAND_BYTE_POLARITY_1,
  COMMAND_BYTE_CONFIG_0,
  COMMAND_BYTE_CONFIG_1
};


/**
 * Write out each device register map.  This includes 
 * configuration, polarity and output registers.
 *
 * @return error handling result code.
 */
static aioc_error_t aioc_i2c_gpio_write_device_maps(void);

/**
 * Write data value to a device register per the device address and
 * register command.
 *
 * @param device_address is the device's i2c address.
 *
 * @param command_byte is basically a register address within a device.
 *
 * @param data is the register data to write out.
 *
 * @return error handling result code.
 */
static aioc_error_t aioc_i2c_gpio_register_write(
  uint32_t device_address,
  uint32_t command_byte,
  uint8_t data);

//==============================================================================
//==============================================================================


//================================
// Public  definitions.
//================================

//==============================================================================
//==============================================================================
aioc_error_t
aioc_i2c_gpio_configure(void)
{
  i2c_gpio_pin_conf_t* pin_conf = 0;
  uint32_t i = 0;
  uint32_t device = 0;
  uint32_t bank = 0;
  uint32_t i2c_gpio_pin_configuration_table_size = 0;
  aioc_error_t e;
  
  // Pupulate each device's register map then write it out to the devices.
  memset(&i2c_gpio_devices, 0, sizeof(i2c_gpio_devices));
  
  i2c_gpio_pin_configuration_table_size = 
            (sizeof(i2c_gpio_pin_configuration_table)/
            sizeof(*i2c_gpio_pin_configuration_table));

  for ( i = 0; i < i2c_gpio_pin_configuration_table_size; i++)
  {
    pin_conf = &i2c_gpio_pin_configuration_table[i];
    
    device = pin_conf->device;
    bank = pin_conf->bank;
    
    i2c_gpio_devices[device].regs.configuration[bank]  |=
      (uint8_t)((pin_conf->configuration & 0x1) << pin_conf->pin); 
    i2c_gpio_devices[device].regs.polarity[bank]     |=
      (uint8_t)((pin_conf->polarity & 0x1) << pin_conf->pin);
    if (pin_conf->configuration == 0)
    {
      i2c_gpio_devices[device].regs.output[bank]   |=
        (uint8_t)((pin_conf->initial_value & 0x1) << pin_conf->pin); 
    }
  }
  
  e = aioc_i2c_gpio_write_device_maps();
  if (e)  {  return e;  }
  
  return error_none;
}



//==============================================================================
//==============================================================================
aioc_error_t
aioc_i2c_gpio_pin_pulse_low(
  i2c_gpio_pin_name_t pin_name, 
  uint32_t pulse_duration) 
{
  aioc_error_t e;

  if (pin_name >= NUMBER_OF_GPIO_PINS)
  {
    return error_bad_param;
  }
  
  e = aioc_util_i2c_open();
  if (e)  {  return e;  }
  
  // Get the device address, command byte and register data.  Make a backup of the
  // data.  Then set the pin bit low in the data.  Then write out the data.
  // Then perform the delay.  Then write out the backup.
  {
    i2c_gpio_pin_conf_t*  pin_conf = &i2c_gpio_pin_configuration_table[pin_name];
    uint32_t              device_adrs = pin_conf->device_adrs;
    uint32_t              device = pin_conf->device;
    uint32_t              bank = pin_conf->bank;
    uint32_t              pin = pin_conf->pin;
    uint32_t              command_byte = COMMAND_BYTE_OUTPUT_0 + bank;
    uint8_t               data = 0;
    
    data = i2c_gpio_devices[device].regs.output[bank];
    data = data & (uint8_t)((~(1 << pin)) & 0xFF);
    e = aioc_i2c_gpio_register_write(
          device_adrs,
          command_byte,
          data);
    if (e)  {  return e;  }
    
    aioc_util_delay_ns(pulse_duration);
    
    data = i2c_gpio_devices[device].regs.output[bank];
    e = aioc_i2c_gpio_register_write(
          device_adrs,
          command_byte,
          data);
    if (e)  {  return e;  }
  }
  
  e = aioc_util_i2c_close();
  if (e)  {  return e;  }
  
  return error_none;
}

//==============================================================================
//==============================================================================
aioc_error_t
aioc_i2c_gpio_pin_level_set( i2c_gpio_pin_name_t pin_name, uint32_t level) 
{
  aioc_error_t e;

  if (pin_name >= NUMBER_OF_GPIO_PINS)
  {
    return error_bad_param;
  }
  
  e = aioc_util_i2c_open();
  if (e)  {  return e;  }
  
  {
    i2c_gpio_pin_conf_t*  pin_conf = &i2c_gpio_pin_configuration_table[pin_name];
    uint32_t              device_adrs = pin_conf->device_adrs;
    uint32_t              device = pin_conf->device;
    uint32_t              bank = pin_conf->bank;
    uint32_t              pin = pin_conf->pin;
    uint32_t              command_byte = COMMAND_BYTE_OUTPUT_0 + bank;
    uint8_t               data = 0;
    
    data = i2c_gpio_devices[device].regs.output[bank];
    if (level)
    {
      data = BIT_SET(data, pin);
    }
    else
    {
      data = BIT_CLR(data, pin);
    }
    i2c_gpio_devices[device].regs.output[bank] = data;
    
    e = aioc_i2c_gpio_register_write(
          device_adrs,
          command_byte,
          data);
    if (e)  {  return e;  }   
  }
  
  e = aioc_util_i2c_close();
  if (e)  {  return e;  }
  
  return error_none;
}



//================================
// Private definitions.
//================================

//==============================================================================
//==============================================================================
static aioc_error_t
aioc_i2c_gpio_write_device_maps(void)
{
  uint32_t i;
  uint32_t device_address = 0;
  aioc_error_t e = error_none;
  
  e = aioc_util_i2c_open();
  if (e)  {  return e;  }
  
  for ( i = 0; i < NUM_DEVICES; i++)
  {
    device_address = DEVICE_ADRS_BASE + i;
    
    e = aioc_i2c_gpio_register_write(
          device_address,
          COMMAND_BYTE_CONFIG_0,
          i2c_gpio_devices[i].regs.configuration[0]);
    if (e)  {  return e;  }
    
    e = aioc_i2c_gpio_register_write(
          device_address,
          COMMAND_BYTE_CONFIG_1,
          i2c_gpio_devices[i].regs.configuration[1]);
    if (e)  {  return e;  }
    
    e = aioc_i2c_gpio_register_write(
          device_address,
          COMMAND_BYTE_POLARITY_0,
          i2c_gpio_devices[i].regs.polarity[0]);
    if (e)  {  return e;  }
    
    e = aioc_i2c_gpio_register_write(
          device_address,
          COMMAND_BYTE_POLARITY_1,
          i2c_gpio_devices[i].regs.polarity[1]);
    if (e)  {  return e;  }
    
    e = aioc_i2c_gpio_register_write(
          device_address,
          COMMAND_BYTE_OUTPUT_0,
          i2c_gpio_devices[i].regs.output[0]);
    if (e)  {  return e;  }
    
    e = aioc_i2c_gpio_register_write(
          device_address,
          COMMAND_BYTE_OUTPUT_1,
          i2c_gpio_devices[i].regs.output[1]);
    if (e)  {  return e;  }
  }
  
  e = aioc_util_i2c_close();
  if (e)  {  return e;  }
  
  return error_none;
}

//==============================================================================
//==============================================================================
static aioc_error_t
aioc_i2c_gpio_register_write(
  uint32_t device_address,
  uint32_t command_byte,
  uint8_t data)
{
  aioc_error_t  e = 0;

  e = aioc_util_i2c_write(device_address, command_byte, &data, 1);
  if (e)  {  return e;  }
  
  return error_none;
}















