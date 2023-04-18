#ifndef AIOC_I2C_GPIO_H
#define AIOC_I2C_GPIO_H

// MERCURY HEADER GOES HERE
// TBD

/**
 * @file:    aioc_i2c_gpio.h
 * @author:  Russell Shahenian
 * @since:   4/4/23
 * @brief:   This file contains the public AIOC i2c_gpio module specifications.
 */

#include "aioc_defs.h"

#include <stdint.h>

//================================
// Public specifications.
//================================

// The comments below represent a table of configurations per each 
// I2C GPIO pin.
// The configuration procedure will use 'each line' in the table to setup
// the GPIO's.
// The table is broken up in the code into an enum type for the names and a
// table of structures for the config values.  See below.
// For further details please refer to 
//    1) the AIOC schematic's I2C GPIO EXPANDERS pages 1 and 2, and
//    2) the TI TCA9555 datasheet.

/*******************************************************************************
SIGNAL                   I2C_ADRS         PORT       PIN       DIR     POL     INIT
A5V_SW_BANK1_A0          0X20             0          0         0       0       0
A5V_SW_BANK2_A0          0X20             0          1         0       0       0
A5V_SW_BANK1_A1          0X20             0          2         0       0       0
A5V_SW_BANK2_A1          0X20             0          3         0       0       0
A5V_SW_BANK1_EN          0X20             0          4         0       0       1
A5V_SW_BANK2_EN          0X20             0          5         0       0       1
A5V_3V3_ADC_RESET_N      0X20             0          6         0       0       1
A5V_3V3_ADC_BUSY         0X20             0          7         1       0       0

A7V_SW_BANK1_A0          0X20             1          0         0       0       0
A7V_SW_BANK2_A0          0X20             1          1         0       0       0
A7V_SW_BANK1_A1          0X20             1          2         0       0       0
A7V_SW_BANK2_A1          0X20             1          3         0       0       0
A7V_SW_BANK1_EN          0X20             1          4         0       0       1
A7V_SW_BANK2_EN          0X20             1          5         0       0       1
A7V_3V3_ADC_RESET_N      0X20             1          6         0       0       1
A7V_3V3_ADC_BUSY         0X20             1          7         1       0       0


A95mV_SW_BANK1_A0        0X21             0          0         0       0       0
A95mV_SW_BANK2_A0        0X21             0          1         0       0       0
A95mV_SW_BANK1_A1        0X21             0          2         0       0       0
A95mV_SW_BANK2_A1        0X21             0          3         0       0       0
A95mV_SW_BANK1_EN        0X21             0          4         0       0       1
A95mV_SW_BANK2_EN        0X21             0          5         0       0       1
A95mV_3V3_ADC_RESET_N    0X21             0          6         0       0       1
A95mV_3V3_ADC_BUSY       0X21             0          7         1       0       0

ARTD_SW_A0               0X21             1          0         0       0       0
ARTD_SW_A1               0X21             1          1         0       0       0
ARTD_SW_EN               0X21             1          2         0       0       1
ARTD_3V3_ADC_RESET_N     0X21             1          3         0       0       1
ARTD_3V3_ADC_BUSY        0X21             1          4         1       0       0
EP10V_3V3_ADC_RESET_N    0X21             1          5         0       0       1
EP10V_3V3_ADC_BUSY       0X21             1          6         1       0       0
NC_0                     0X21             1          7         1       0       0


A10V_SW_A0               0X22             0          0         0       0       0
A10V_SW_A1               0X22             0          1         0       0       0
A10V_SW_EN               0X22             0          2         0       0       1
A5V_ADC_PG               0X22             0          3         1       0       0
A7V_ADC_PG               0X22             0          4         1       0       0
A95mV_ADC_PG             0X22             0          5         1       0       0
ARTD_ADC_PG              0X22             0          6         1       0       0
EP10V_ADC_PG             0X22             0          7         1       0       0

AZ_EL_SW_A0              0X22             1          0         0       0       0
AZ_EL_SW_A1              0X22             1          1         0       0       0
AZ_EL_SW_EN              0X22             1          2         0       0       1
RTD_EXC_00_OFF_ON_N      0X22             1          3         0       0       1
RTD_EXC_01_OFF_ON_N      0X22             1          4         0       0       1
RTD_EXC_02_OFF_ON_N      0X22             1          5         0       0       1
RTD_EXC_03_OFF_ON_N      0X22             1          6         0       0       1
RTD_EXC_04_OFF_ON_N      0X22             1          7         0       0       1




EP10V_00_EN              0x23             0          0         0       0       0
EP10V_01_EN              0x23             0          1         0       0       0
EP10V_02_EN              0x23             0          2         0       0       0
EP10V_03_EN              0x23             0          3         0       0       0
EP10V_04_EN              0x23             0          4         0       0       0
EP10V_05_EN              0x23             0          5         0       0       0
EP10V_06_EN              0x23             0          6         0       0       0
EP10V_07_EN              0x23             0          7         0       0       0

EP10V_08_EN              0x23             1          0         0       0       0
EP10V_09_EN              0x23             1          1         0       0       0
EP10V_10_EN              0x23             1          2         0       0       0
EP10V_11_EN              0x23             1          3         0       0       0
P10V_POWER_EN            0x23             1          4         0       0       0
NC_1                     0x23             1          5         1       0       0
NC_2                     0x23             1          6         1       0       0
NC_3                     0x23             1          7         1       0       0


EP10V_00_PG_N            0X24             0          0         1       0       0
EP10V_01_PG_N            0X24             0          1         1       0       0
EP10V_02_PG_N            0X24             0          2         1       0       0
EP10V_03_PG_N            0X24             0          3         1       0       0
EP10V_04_PG_N            0X24             0          4         1       0       0
EP10V_05_PG_N            0X24             0          5         1       0       0
EP10V_06_PG_N            0X24             0          6         1       0       0
EP10V_07_PG_N            0X24             0          7         1       0       0

EP10V_08_PG_N            0X24             1          0         1       0       0
EP10V_09_PG_N            0X24             1          1         1       0       0
EP10V_10_PG_N            0X24             1          2         1       0       0
EP10V_11_PG_N            0X24             1          3         1       0       0
P10V_PG_0                0X24             1          4         1       0       0
P10V_PG_1                0X24             1          5         1       0       0
NC_4                     0X24             1          6         1       0       0
NC_5                     0X24             1          7         1       0       0


EP10V_00_FLG_N           0X25             0          0         1       0       0
EP10V_01_FLG_N           0X25             0          1         1       0       0
EP10V_02_FLG_N           0X25             0          2         1       0       0
EP10V_03_FLG_N           0X25             0          3         1       0       0
EP10V_04_FLG_N           0X25             0          4         1       0       0
EP10V_05_FLG_N           0X25             0          5         1       0       0
EP10V_06_FLG_N           0X25             0          6         1       0       0
EP10V_07_FLG_N           0X25             0          7         1       0       0

EP10V_08_FLG_N           0X25             1          0         1       0       0
EP10V_09_FLG_N           0X25             1          1         1       0       0
EP10V_10_FLG_N           0X25             1          2         1       0       0
EP10V_11_FLG_N           0X25             1          3         1       0       0
NC_6                     0X25             1          4         1       0       0
NC_7                     0X25             1          5         1       0       0
NC_8                     0X25             1          6         1       0       0
NC_9                     0X25             1          7         1       0       0
*******************************************************************************/



typedef enum
{
  // U149
  A5V_SW_BANK1_A0,
  A5V_SW_BANK2_A0,
  A5V_SW_BANK1_A1,
  A5V_SW_BANK2_A1,
  A5V_SW_BANK1_EN,
  A5V_SW_BANK2_EN,
  A5V_3V3_ADC_RESET_N,
  A5V_3V3_ADC_BUSY,

  A7V_SW_BANK1_A0,
  A7V_SW_BANK2_A0,
  A7V_SW_BANK1_A1,
  A7V_SW_BANK2_A1,
  A7V_SW_BANK1_EN,
  A7V_SW_BANK2_EN,
  A7V_3V3_ADC_RESET_N,
  A7V_3V3_ADC_BUSY,

  // U150
  A95mV_SW_BANK1_A0,
  A95mV_SW_BANK2_A0,
  A95mV_SW_BANK1_A1,
  A95mV_SW_BANK2_A1,
  A95mV_SW_BANK1_EN,
  A95mV_SW_BANK2_EN,
  A95mV_3V3_ADC_RESET_N,
  A95mV_3V3_ADC_BUSY,

  ARTD_SW_A0,
  ARTD_SW_A1,
  ARTD_SW_EN,
  ARTD_3V3_ADC_RESET_N,
  ARTD_3V3_ADC_BUSY,
  EP10V_3V3_ADC_RESET_N,
  EP10V_3V3_ADC_BUSY,
  NC_0, 

  // U151
  A10V_SW_A0,
  A10V_SW_A1,
  A10V_SW_EN,
  A5V_ADC_PG,
  A7V_ADC_PG,
  A95mV_ADC_PG,
  ARTD_ADC_PG,
  EP10V_ADC_PG,

  AZ_EL_SW_A0,
  AZ_EL_SW_A1,
  AZ_EL_SW_EN,
  RTD_EXC_00_OFF_ON_N,
  RTD_EXC_01_OFF_ON_N,
  RTD_EXC_02_OFF_ON_N,
  RTD_EXC_03_OFF_ON_N,
  RTD_EXC_04_OFF_ON_N,



  // U152
  EP10V_00_EN,
  EP10V_01_EN,
  EP10V_02_EN,
  EP10V_03_EN,
  EP10V_04_EN,
  EP10V_05_EN,
  EP10V_06_EN,
  EP10V_07_EN,

  EP10V_08_EN,
  EP10V_09_EN,
  EP10V_10_EN,
  EP10V_11_EN,
  P10V_POWER_EN,  
  NC_1,      
  NC_2,      
  NC_3,      

  // U153
  EP10V_00_PG_N,
  EP10V_01_PG_N,
  EP10V_02_PG_N,
  EP10V_03_PG_N,
  EP10V_04_PG_N,
  EP10V_05_PG_N,
  EP10V_06_PG_N,
  EP10V_07_PG_N,

  EP10V_08_PG_N,
  EP10V_09_PG_N,
  EP10V_10_PG_N,
  EP10V_11_PG_N,
  P10V_PG_0,     
  P10V_PG_1,     
  NC_4,         
  NC_5,         

  // U154
  EP10V_00_FLG_N,
  EP10V_01_FLG_N,
  EP10V_02_FLG_N,
  EP10V_03_FLG_N,
  EP10V_04_FLG_N,
  EP10V_05_FLG_N,
  EP10V_06_FLG_N,
  EP10V_07_FLG_N,

  EP10V_08_FLG_N,
  EP10V_09_FLG_N,
  EP10V_10_FLG_N,
  EP10V_11_FLG_N,
  NC_6,       
  NC_7,       
  NC_8,       
  NC_9,       

  NUMBER_OF_GPIO_PINS
}
i2c_gpio_pin_name_t;


enum aioc_i2c_gpio_direction
{
  AIOC_I2C_GPIO_DIRECTION_OUTPUT = 0,
  AIOC_I2C_GPIO_DIRECTION_INPUT = 1
}




/**
 * Sets the specified pin to the specified direction.
 *
 * @param pin is the number of the specified pin.
 *
 * @param direction is the specified pin direction.
 *
 * @return error handling result code.
 */
aioc_error_t aioc_i2c_gpio_direction_set(
  i2c_gpio_pin_name_t pin,
  enum aioc_i2c_gpio_direction direction);
  

/**
 * Sets the specified pin to the specified direction.
 *
 * @param pin is the number of the specified pin.
 *
 * @param value is the specified pin output value.
 *
 * @return error handling result code.
 */
aioc_error_t aioc_i2c_gpio_output_set(i2c_gpio_pin_name_t pin, uint32_t value);


/**
 * Initial configuration of the AIOC GPIO expanders per the configuration table
 * above.
 *
 * @return error handling result code.
 */
aioc_error_t aioc_i2c_gpio_configure(void);

/**
 * Pulse the specified pin low for the specified number of nannoseconds.
 *
 * @param pin_name is the specified pin.
 *
 * @param pulse_duration is the specified count of nanoseconds.
 *
 * @return error handling result code.
 */
aioc_error_t aioc_i2c_gpio_pin_pulse_low(
  i2c_gpio_pin_name_t line, 
  uint32_t pulse_duration);

/**
 * Set the specified pin to the specified level.
 *
 * @param pin_name is the specified pin.
 *
 * @param level is the specified level.
 *
 * @return error handling result code.
 */
aioc_error_t aioc_i2c_gpio_pin_level_set(
  i2c_gpio_pin_name_t line, 
  uint32_t level);


#endif  // AIOC_I2C_GPIO_H















