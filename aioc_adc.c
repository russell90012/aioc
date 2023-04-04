//==============================================================================
//==============================================================================


#include "aioc_adc.h"

#include "aioc_util.h"


//==========================
// Private specifications.
//==========================

#define VENDOR_L_adrs   0x000C
#define VENDOR_L_reset  0x56



#define VENDOR_H_adrs   0x000D
#define VENDOR_H_reset  0x04



#define STATUS_adrs                   0x0014

#define STATUS_RESET_FLAG_mask        0x1
#define STATUS_RESET_FLAG_offset      5
#define STATUS_RESET_FLAG_read(data)  \
        (uint8_t)((data & (STATUS_RESET_FLAG_mask << STATUS_RESET_FLAG_offset)) \
        >> STATUS_RESET_FLAG_offset)
        
#define STATUS_SPI_ERROR_mask        0x1
#define STATUS_SPI_ERROR_offset      2
#define STATUS_SPI_ERROR_read(data)  \
        (uint8_t)((data & (STATUS_SPI_ERROR_mask << STATUS_SPI_ERROR_offset)) \
        >> STATUS_SPI_ERROR_offset)

#define DEVICE_TYPE_adrs    0x0003
#define DEVICE_TYPE_reset   0x07



#define SCRATCH_PAD_adrs                   0x000A



#define SETUP_adrs                   0x0020

#define SETUP_CYC_CTRL_mask           0x1
#define SETUP_CYC_CTRL_offset         1
        
#define SETUP_SPI_MODE_mask          0x1
#define SETUP_SPI_MODE_offset        2



#define SEQ_CTRL_adrs                   0x0022



#define GPIO_CTRL_adrs    0x0026
#define GPIO_CTRL_GPO0_EN_offset    0
#define GPIO_CTRL_GPO0_EN_mask      1



#define GP_MODE_adrs                  0x0027
#define GP_MODE_BUSY_GP_SEL_offset    5
#define GP_MODE_BUSY_GP_SEL_mask      0x1
#define GP_MODE_BUSY_GP_EN_offset     1
#define GP_MODE_BUSY_GP_EN_mask       0x1

      

#define ADC_REGISTER_SIZE_MAX 32
#define ADC_INSTRUCTION_SIZE  2

#define CONVERSION_MODE_COMMAND_channel_selection(channel_number) \
          (0x10+(channel_number & 0xF))
#define CONVERSION_MODE_COMMAND_register_configuration_mode  (0xA)



/**
 * Select the ADC input channel to perform the next conversion upon.
 *
 * @param input is input channel.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
aioc_error_t aioc_adc_conversion_mode_command_channel_selection(
  uint32_t input);

/**
 * Check vendor and device id register values for expected values.  Check that
 * the scratch pad register works as expected.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
aioc_error_t aioc_adc_self_check(void);

/**
 * Program the channel sequencing to for single-cycle mode.  And allocate
 * the busy (converting) signal to the ADC's GP0.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
aioc_error_t aioc_adc_configure_single_cycle_mode(void);

/**
 * Put the ADC into Conversion mode.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
aioc_error_t aioc_adc_to_conversion_mode(void);

/**
 * Put the ADC into Registger Configuration mode.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
aioc_error_t aioc_adc_to_register_mode(void);


/**
 * Write conversion mode commands to ADC.
 *
 * @param command is the the command that's written.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
aioc_error_t aioc_adc_conversion_mode_command_issue(uint32_t command);

/**
 * Read conversion result from ADC whilest in conversion mode.
 *
 * @param result is how to conversion result is returned to calle.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
aioc_error_t aioc_adc_conversion_mode_result_read(uint16_t* result);


/**
 * Write the data to the ADC register address specified.
 *
 * @param register_adrs is the specified ADC register address.
 *
 * @param data is the data being written.
 *
 * @param data_count is the number of bytes being written.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 *
 * @note the ADC must be in Register Configuration mode for this to work.
 */
aioc_error_t aioc_adc_register_write(
  uint32_t register_adrs,
  uint8_t* data,
  uint32_t data_count);

/**
 * Read data from the specified ADC register address.
 *
 * @param register_adrs is the specified ADC register address.
 *
 * @param data is the data being read.
 *
 * @param data_count is the number of bytes being read.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 *
 * @note the ADC must be in Register Configuration mode for this to work.
 */
aioc_error_t aioc_adc_register_read(
  uint32_t register_adrs,
  uint8_t* data,
  uint32_t data_count);


// Enumeration of the ADC modes (and reset is not really one of them).
typedef enum
{
  AIOC_ADC_STATE_RESET = 0,
  AIOC_ADC_STATE_REGISTER_MODE = 1,
  AIOC_ADC_STATE_CONVERSION_MODE
}
aioc_adc_state_t;

// The ADC context is used to hold ADC specific into.
typedef struct
{
  aioc_adc_state_t state;
  uint32_t spi_dev_id;
  uint32_t spi_cs_id;
  uint32_t gpio_id;
}
aioc_adc_context_t;

// ADC context info per AIOC ADC.
aioc_adc_context_t aioc_adc_context_table[NUMBER_OF_AIOC_ADC_IDS] =
{
  {AIOC_ADC_STATE_RESET, 0, 0, 0}, // AIOC_ADC_ID_5V
  {AIOC_ADC_STATE_RESET, 0, 1, 0}, // AIOC_ADC_ID_7V
  {AIOC_ADC_STATE_RESET, 1, 0, 1}, // AIOC_ADC_ID_95mV
  {AIOC_ADC_STATE_RESET, 1, 1, 1}, // AIOC_ADC_ID_RTD
  {AIOC_ADC_STATE_RESET, 1, 2, 1}  // AIOC_ADC_ID_EP10
};

//==========================
//==========================


//================================
// Public function definitions.
//================================

//==============================================================================
//==============================================================================
aioc_error_t
aioc_adc_init(aioc_adc_id_t aioc_adc_id, aioc_adc_handle_t* aioc_adc_handle)
{
  aioc_error_t e = error_none;

  *aioc_adc_handle = 0;
 
  if (aioc_adc_id >= NUMBER_OF_AIOC_ADC_IDS)
  {
    return error_bad_param;
  }

  aioc_adc_context_t* aioc_adc_context =  &aioc_adc_context_table[aioc_adc_id];

  aioc_adc_context->state = AIOC_ADC_STATE_REGISTER_MODE;

  e = aioc_util_spi_open(
        aioc_adc_context->spi_dev_id,
        aioc_adc_context->spi_cs_id);
  if (e)  {  return e;  }
 
  // Perform ADC self-check.
  e = aioc_adc_self_check();  
  if (e)  {  return e;  }
 
  // Configure the ADC
  e = aioc_adc_configure_single_cycle_mode();
  if (e)  {  return e;  }
  
  // Start conversion mode.
  e = aioc_adc_to_conversion_mode();
  if (e)  {  return e;  }
    
  e = aioc_util_spi_close();
  if (e)  {  return e;  }

  // Promote ADC state to conversion mode and pass back
  // context pointer as a handle.
  aioc_adc_context->state = AIOC_ADC_STATE_CONVERSION_MODE;
  *aioc_adc_handle = (aioc_adc_handle_t)aioc_adc_context;
 
  return error_none;
}

//==============================================================================
//==============================================================================
aioc_error_t 
aioc_adc_convert_single_cycle(
  aioc_adc_handle_t adc_handle,
  aioc_adc_input_t adc_input,
  uint16_t* result)
{
  aioc_error_t e = error_none;
  aioc_adc_context_t* aioc_adc_context = (aioc_adc_context_t*)adc_handle;

  // Check that we're in conversion mode.
  if (aioc_adc_context->state != AIOC_ADC_STATE_CONVERSION_MODE)
  {
    return error_bad_state;
  }

  e = aioc_util_spi_open(
        aioc_adc_context->spi_dev_id,
        aioc_adc_context->spi_cs_id);
  if (e)  {  return e;  }
  
  // Issue specific input channel selection command.
  e = aioc_adc_conversion_mode_command_channel_selection(adc_input);
  if (e)  {  return e;  }
  
  // Pulse the appropriate ADC convert signal active (low) for appropriate
  // duration.
  //  CNV Low Time: tCNVL: 80 ns
  e = aioc_util_ultrascale_gpio_pulse_low(aioc_adc_context->gpio_id, 80);
  if (e)  {  return e;  }

  // Delay for conversion time.
  //  Conversion Time: tCONVERT: 380-415 ns.
  aioc_util_delay_ns(500);
  
  // Read the conversion result.
  e = aioc_adc_conversion_mode_result_read(result);
  if (e)  {  return e;  }
  
  e = aioc_util_spi_close();
  if (e)  {  return e;  }

  return error_none;
}


//================================
// Private function definitions.
//================================

//==============================================================================
//==============================================================================
aioc_error_t
aioc_adc_self_check(void)
{
  aioc_error_t e = error_none;
  uint8_t data;

  
  // Check the Device Status Register for reset flag or spi error.
  data = 0;
  e = aioc_adc_register_read(STATUS_adrs, &data, 1);
  if (e)  {  return e;  }
  if (STATUS_RESET_FLAG_read(data))
  {
    return error_adc_self_check;
  }  
  if (STATUS_SPI_ERROR_read(data))
  {
    return error_adc_self_check;
  }
  
  // Check the Vendor ID of the ADC.
  //  Read vendor id low register and check the id.
  //  Read vendor id high register and check the id.
  e = aioc_adc_register_read(VENDOR_L_adrs, &data, 1);
  if (e)  {  return e;  }
  if (data != VENDOR_L_reset)
  {
    return error_adc_self_check;
  }
  data = 0;
  aioc_adc_register_read(VENDOR_H_adrs, &data, 1);
  if (e)  {  return e;  }
  if (data != VENDOR_H_reset)
  {
    return error_adc_self_check;
  }

  // Check the Device Type of the ADC.
  //  Read device type register and check the device type.
  data = 0;
  e = aioc_adc_register_read(DEVICE_TYPE_adrs, &data, 1);
  if (e)  {  return e;  }
  if (data != DEVICE_TYPE_reset)
  {
    return error_adc_self_check;
  }

   // Check scratch pad register access.
  //  Write 0xA5 to scratch pad register then read it back.
  data = 0xA5;
  e = aioc_adc_register_write(SCRATCH_PAD_adrs, &data, 1);
  if (e)  {  return e;  }
  data = 0;
  e = aioc_adc_register_read(SCRATCH_PAD_adrs, &data, 1);
  if (e)  {  return e;  }
  if (data != 0xA5)
  {
    return error_adc_self_check;
  }

  return error_none;
}

//==============================================================================
//==============================================================================
aioc_error_t
aioc_adc_configure_single_cycle_mode(void)
{
  aioc_error_t e = error_none;
  uint8_t data;
  
  // Setup the channel sequencing for Single-cycle mode.
  //
  // When single-cycle command mode is enabled, the first analog input
  // channel selected is the one specified in the AS_SLOT0 register.
  // The channel only updates after a valid command is received.
  //
  // To enable single-cycle command mode, set the STD_SEQ_EN
  // bit to 0, set the NUM_SLOTS_AS bit field to 0x00, and set the
  // CYC_CTRL bit to 1 (see Table 15).
  //
  // When single-cycle command mode is enabled, the configuration
  // settings for each channel are set with the corresponding CONFIG_INn register.
  // Therefore, all analog inputs can have different
  // channel configuration options, analog input high-Z mode enable
  // settings, and threshold detection enable settings. Configure each
  // CONFIG_INn register before entering conversion mode and initiating conversions.

  // Setup channel sequencing for Single-Cycle command mode.
  // Set the STD_SEQ_EN bit to 0, set the NUM_SLOTS_AS bit field to 0x00, and
  data = 0;
  e = aioc_adc_register_write(SEQ_CTRL_adrs, &data, 1);
  if (e)  {  return e;  }
 
  // Set the CYC_CTRL bit to 1.
  data = 0;
  e = aioc_adc_register_read(SETUP_adrs, &data, 1);
  if (e)  {  return e;  }
  data |=  ((uint8_t)(SETUP_CYC_CTRL_mask << SETUP_CYC_CTRL_offset));
  e = aioc_adc_register_write(SETUP_adrs, &data, 1);
  if (e)  {  return e;  }
  
  // Setup the busy signal on GP0.
  //
  // When the busy indicator is enabled on a general-purpose pin, the
  // selected general-purpose pin is driven high while the ADC is in
  // the conversion phase, and transitions low when the ADC result is
  // ready. Set the BUSY_GP_EN bit in the GP_MODE register to 1 to
  // enable the busy indicator on the selected general-purpose pin.
  //
  // The busy indicator
  // can be enabled on either the BSY_ALT_GP0 pin or the GP3 pin.
  // The BUSY_GP_SEL bit in the GP_MODE register selects which of
  // the general-purpose pins is configured as the busy indicator. Set
  // the BUSY_GP_EN bit in the GP_MODE register to 1 to enable the
  // busy indicator on the selected general-purpose pin.

  // Enable the busy indicator the on BSY_ALT_GP0 pin of the 
  // GPIO_CTRL register.
  data = 0;
  e = aioc_adc_register_read(GPIO_CTRL_adrs, &data, 1);
  if (e)  {  return e;  }
  data |= ((uint8_t)(GPIO_CTRL_GPO0_EN_mask << GPIO_CTRL_GPO0_EN_offset));
  e = aioc_adc_register_write(GPIO_CTRL_adrs, &data, 1);
  if (e)  {  return e;  }
  
  // Select BSY_ALT_GP0 pin to configure as the busy indicator with the 
  // BUSY_GP_SEL bit in the GP_MODE register.
  // 0: configures BSY_ALT_GP0 as the busy indicator when the BUSY_GP_EN bit
  // is set to 1.
  // 1: configures GP3 as the busy indicator when the BUSY_GP_EN bit
  // is set to 1
  // Set the BUSY_GP_EN bit in the GP_MODE register to 1 to enable the 
  // busy indicator on the BSY_ALT_GP0 pin.  
  // 0: busy indicator on the general-purpose pin function disabled.
  // 1: busy indicator on the general-purpose pin function enabled.
  // GP_MODE reset: 0x0
  data = 0;
  e = aioc_adc_register_read(GP_MODE_adrs, &data, 1);
  if (e)  {  return e;  }
  data |=  (uint8_t)(GP_MODE_BUSY_GP_EN_mask  << GP_MODE_BUSY_GP_EN_offset);
  e = aioc_adc_register_write(GP_MODE_adrs, &data, 1);
  if (e)  {  return e;  }
  
  return error_none;
}

//==============================================================================
//==============================================================================
// Issue specific input channel selection command.
aioc_error_t
aioc_adc_conversion_mode_command_channel_selection(uint32_t input)
{
  aioc_error_t e = error_none;
  
  // Issue specific input channel selection command.
  e = aioc_adc_conversion_mode_command_issue(
        CONVERSION_MODE_COMMAND_channel_selection(input));
  if (e)  {  return e;  }
  
  return error_none;
}

//==============================================================================
// Read 16 bit result from ADC.
// NOTE: Must be in conversion mode for this to work.
//==============================================================================
// TBD
// Endianess????
aioc_error_t
aioc_adc_conversion_mode_result_read(uint16_t* result)
{
  aioc_error_t e = error_none;
  uint8_t data[2] = {0, 0};
  
  // Read 16 bits.
  e = aioc_util_spi_read(data, 2);
  if (e)  {  return e;  }
  
  *result = (uint16_t)((data[1] << 8)| data[0]);
  
  return error_none;  
}

//==============================================================================
//==============================================================================
aioc_error_t
aioc_adc_to_conversion_mode(void)
{
  aioc_error_t e = error_none;
  uint8_t data = 0;
  
  // Put the ADC into Conversion mode.
  data = 0;
  e = aioc_adc_register_read(SETUP_adrs, &data, 1);
  if (e)  {  return e;  }
  data |=  (uint8_t)(SETUP_SPI_MODE_mask << SETUP_SPI_MODE_offset);
  e = aioc_adc_register_write(SETUP_adrs, &data, 1);
  if (e)  {  return e;  }
  
  // Delay for transit from Register mode to Converstion mode.
  //  The digital host must provide a delay specified by tSCKCNV after
  //  the final SCK rising edge of the register write before initiating
  //  conversions with a CNV rising edge.
  //  Last SCK Edge to CNV Rising Edge Delay: tSCKCNV: 80 ns
  aioc_util_delay_ns(80);

  return error_none;
}

//==============================================================================
//==============================================================================
aioc_error_t
aioc_adc_to_register_mode(void)
{
  aioc_error_t e = error_none;
  uint8_t data = 0;
  
  // Put the ADC into Register configuration mode.
  // NOTE: This ASSUMES the ADC is in Conversion mode.
  e = aioc_adc_conversion_mode_command_issue(
        CONVERSION_MODE_COMMAND_register_configuration_mode);
  if (e)  {  return e;  }

  return error_none;
}

//==============================================================================
// Write 5 bit conversion mode command to ADC.
// NOTE: Must be in conversion mode for this to work.
//==============================================================================
// TBD
// Endianess????
aioc_error_t
aioc_adc_conversion_mode_command_issue(uint32_t command)
{
  aioc_error_t e = error_none;
  uint8_t data = 0;
 
  // Put the LSB five LSb's into a byte and SPI write it out.
  data = (uint8_t) (command & 0x1F);
  e = aioc_util_spi_write(&data, 1);
  if (e)  {  return e;  }
   
  return error_none;
}

//==============================================================================
//==============================================================================
static uint8_t register_data[ADC_INSTRUCTION_SIZE + ADC_REGISTER_SIZE_MAX];


// While in Register Configuration Mode, We're using (default):
//  Descending Address Direction,
//  MultiByte Register Access, and
//  Streaming Instruction Mode.
// So, from what I understand, we will pack the lowest buffer bytes with the
// higher significant instruction and data bytes.
// And we need to update the address with an increase for multi-byte register
// accesses and a flag for read instruction.
    
//==============================================================================
//==============================================================================
// TBD
// Endianess????
aioc_error_t
aioc_adc_register_read(
  uint32_t register_adrs,
  uint8_t* data,
  uint32_t data_count)
{
  aioc_error_t e = error_none;
  uint32_t i = 0;

  // Check data count to buffer size minus instruction size (of 2).
  if ((sizeof(register_data) - ADC_INSTRUCTION_SIZE) <  data_count)
  {
    return error_bad_param;
  }

  // Update address including data count and read flag, 
  // and Pack register address.
  if (data_count > 1)
  {
    register_adrs += (data_count - 1);
  }
  register_adrs |= 0x8000;
  
  register_data[0] = (uint8_t)((register_adrs >> 8) & 0xFF);
  register_data[1] = (uint8_t)((register_adrs) & 0xFF);
  
  // Do SPI transaction.
  e = aioc_util_spi_read(register_data, (data_count + ADC_INSTRUCTION_SIZE));
  if (e)  {  return e;  }
  
  // UnPack data.
  for (i = 0; i < data_count; ++i)
  {
    data[i] = register_data[i+2];
  } 
  
  return error_none;
}

//==============================================================================
//==============================================================================
// TBD
// Endianess????
aioc_error_t
aioc_adc_register_write(
  uint32_t register_adrs,
  uint8_t* data,
  uint32_t data_count)
{
  aioc_error_t e = error_none;
  uint32_t i = 0;

  // Check data count to buffer size minus instruction size (of 2).
  if ((sizeof(register_data) - ADC_INSTRUCTION_SIZE) <  data_count)
  {
    return error_bad_param;
  }

  // Update address with data count and Pack register address.
  if (data_count > 1)
  {
    register_adrs += (data_count - 1);
  }
  
  register_data[0] = (uint8_t)((register_adrs >> 8) & 0xFF);
  register_data[1] = (uint8_t)((register_adrs) & 0xFF);
    
  // Pack data.
  for (i = 0; i < data_count; ++i)
  {
    register_data[i+2] = data[i];
  } 

  // Do SPI transaction.
  e = aioc_util_spi_write(register_data, (data_count + ADC_INSTRUCTION_SIZE));
  if (e)  {  return e;  }
  
  return error_none;
}

