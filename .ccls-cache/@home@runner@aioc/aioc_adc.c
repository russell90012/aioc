// MERCURY HEADER GOES HERE
// TBD

#include "aioc_adc.h"
#include "aioc_i2c_gpio.h"
#include "aioc_util.h"

#include <string.h>

//==========================
// Private specifications.
//==========================

/* AD469x registers */
#define VENDOR_L_adrs 0x000C
#define VENDOR_H_adrs 0x000D
#define STATUS_adrs 0x0014
#define DEVICE_TYPE_adrs 0x0003
#define SCRATCH_PAD_adrs 0x000A
#define SETUP_adrs 0x0020
#define SEQ_CTRL_adrs 0x0022
#define GPIO_CTRL_adrs 0x0026
#define GP_MODE_adrs 0x0027

/* AD469x registers reset values*/
#define VENDOR_L_reset 0x56
#define VENDOR_H_reset 0x04
#define DEVICE_TYPE_reset 0x07

/* AD469x_REG_SETUP */
#define AD469x_SETUP_IF_MODE_MASK (0x01 << 2)
#define AD469x_SETUP_IF_MODE_CONV (0x01 << 2)
#define AD469x_SETUP_CYC_CTRL_MASK (0x01 << 1)
#define AD469x_SETUP_CYC_CTRL_SINGLE(x) ((x & 0x01) << 1)

/* AD469x_REG_GP_MODE */
#define AD469x_GP_MODE_BUSY_GP_EN_MASK (0x01 << 1)
#define AD469x_GP_MODE_BUSY_GP_EN(x) ((x & 0x01) << 1)
#define AD469x_GP_MODE_BUSY_GP_SEL_MASK (0x01 << 4)
#define AD469x_GP_MODE_BUSY_GP_SEL(x) ((x & 0x01) << 4)

/* AD469x_REG_SEQ_CTRL */
#define AD469x_SEQ_CTRL_STD_SEQ_EN_MASK (0x01 << 7)
#define AD469x_SEQ_CTRL_STD_SEQ_EN(x) ((x & 0x01) << 7)
#define AD469x_SEQ_CTRL_NUM_SLOTS_AS_MASK (0x7f << 0)
#define AD469x_SEQ_CTRL_NUM_SLOTS_AS(x) ((x & 0x7f) << 0)

/* AD469x Status Register Masks */
#define STATUS_RESET_FLAG_mask 0x1
#define STATUS_RESET_FLAG_offset 5
#define STATUS_RESET_FLAG_read(data)                                           \
  (uint8_t)((data & (STATUS_RESET_FLAG_mask << STATUS_RESET_FLAG_offset)) >>   \
            STATUS_RESET_FLAG_offset)

#define STATUS_SPI_ERROR_mask 0x1
#define STATUS_SPI_ERROR_offset 2
#define STATUS_SPI_ERROR_read(data)                                            \
  (uint8_t)((data & (STATUS_SPI_ERROR_mask << STATUS_SPI_ERROR_offset)) >>     \
            STATUS_SPI_ERROR_offset)

// Some useful macros.
#define ADC_REGISTER_SIZE_MAX 32
#define ADC_INSTRUCTION_SIZE 2

#define CONVERSION_MODE_COMMAND_channel_selection(channel_number)              \
  (0x10 + (channel_number & 0xF))
#define CONVERSION_MODE_COMMAND_register_configuration_mode (0xA)

//==============================================================================
//==============================================================================

/**
 * Perform a hard reset on the ADC.
 *
 * @param reset_n is the i2c_gpio reset line.
 *
 * @return error handling result code.
 */
static aioc_error_t aioc_adc_reset(i2c_gpio_pin_name_t reset_n);

/**
 * Check vendor and device id register values for expected values.  Check that
 * the scratch pad register works as expected.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
static aioc_error_t aioc_adc_self_check(void);

/**
 * Program the channel sequencing to for single-cycle mode.  And allocate
 * the busy (converting) signal to the ADC's GP0.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
static aioc_error_t aioc_adc_configure_single_cycle_mode(void);

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
static aioc_error_t aioc_adc_register_read(uint32_t register_adrs,
                                           uint8_t *data, uint32_t data_count);

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
static aioc_error_t aioc_adc_register_write(uint32_t register_adrs,
                                            uint8_t *data, uint32_t data_count);

/**
 * Write the data to the ADC register address specified using a mask.
 *
 * @param register_adrs is the specified ADC register address.
 *
 * @param mask is the mask to use.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 *
 * @note the ADC must be in Register Configuration mode for this to work.
 */
static aioc_error_t aioc_adc_register_mask_write(uint32_t register_adrs,
                                                 uint8_t mask, uint8_t data);

/**
 * Enter the ADC into Conversion mode.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
static aioc_error_t aioc_adc_conversion_mode_enter(void);

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
static aioc_error_t
aioc_adc_conversion_mode_command_channel_selection(uint32_t input);

/**
 * Exit the ADC from Conversion Mode.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
static aioc_error_t aioc_adc_conversion_mode_exit(void);

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
static aioc_error_t aioc_adc_conversion_mode_command_issue(uint32_t command);

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
static aioc_error_t aioc_adc_conversion_mode_result_read(uint16_t *result);

// Enumeration of the ADC modes (and reset is not really one of them).
typedef enum {
  AIOC_ADC_STATE_RESET = 0,
  AIOC_ADC_STATE_REGISTER_MODE = 1,
  AIOC_ADC_STATE_CONVERSION_MODE
} aioc_adc_state_t;

// The ADC context is used to hold ADC specific into.
typedef struct {
  aioc_adc_state_t state;
  uint32_t spi_dev_id;
  uint32_t spi_cs_id;
  uint32_t gpio_id;
  i2c_gpio_pin_name_t reset_n;
} aioc_adc_context_t;

// ADC context info per AIOC ADC.
aioc_adc_context_t aioc_adc_context_table[NUMBER_OF_AIOC_ADC_IDS] = {
    {AIOC_ADC_STATE_RESET, 0, 0, 0, A5V_3V3_ADC_RESET_N},   // AIOC_ADC_ID_5V
    {AIOC_ADC_STATE_RESET, 0, 1, 0, A7V_3V3_ADC_RESET_N},   // AIOC_ADC_ID_7V
    {AIOC_ADC_STATE_RESET, 1, 0, 1, A95mV_3V3_ADC_RESET_N}, // AIOC_ADC_ID_95mV
    {AIOC_ADC_STATE_RESET, 1, 1, 1, ARTD_3V3_ADC_RESET_N},  // AIOC_ADC_ID_RTD
    {AIOC_ADC_STATE_RESET, 1, 2, 1, EP10V_3V3_ADC_RESET_N}  // AIOC_ADC_ID_EP10
};

//================================
// Public  definitions.
//================================

//==============================================================================
//==============================================================================
aioc_error_t aioc_adc_init(aioc_adc_id_t aioc_adc_id,
                           aioc_adc_handle_t *aioc_adc_handle) {
  aioc_error_t e = error_none;

  *aioc_adc_handle = 0;

  if (aioc_adc_id >= NUMBER_OF_AIOC_ADC_IDS) {
    return error_bad_param;
  }

  aioc_adc_context_t *aioc_adc_context = &aioc_adc_context_table[aioc_adc_id];

  e = aioc_util_spi_open(aioc_adc_context->spi_dev_id,
                         aioc_adc_context->spi_cs_id);
  if (e) {
    return e;
  }

  // Perform ADC reset.
  e = aioc_adc_reset(aioc_adc_context->reset_n);
  if (e) {
    return e;
  }

  // Perform ADC self-check.
  e = aioc_adc_self_check();
  if (e) {
    return e;
  }

  // Since we passed self check, setup internal state
  // as register configuration mode.
  aioc_adc_context->state = AIOC_ADC_STATE_REGISTER_MODE;

  // Configure the ADC
  e = aioc_adc_configure_single_cycle_mode();
  if (e) {
    return e;
  }

  // Start conversion mode.
  e = aioc_adc_conversion_mode_enter();
  if (e) {
    return e;
  }

  // Since we passed configuration and ADC entered conversion mode,
  // setup internal state to conversion mode.
  // And pass back context pointer as a handle.
  aioc_adc_context->state = AIOC_ADC_STATE_CONVERSION_MODE;
  *aioc_adc_handle = (aioc_adc_handle_t)aioc_adc_context;

  e = aioc_util_spi_close();
  if (e) {
    return e;
  }

  return error_none;
}

//==============================================================================
//==============================================================================
aioc_error_t aioc_adc_convert_single_cycle(aioc_adc_handle_t adc_handle,
                                           aioc_adc_input_t adc_input,
                                           uint16_t *result) {
  aioc_error_t e = error_none;
  aioc_adc_context_t *aioc_adc_context = (aioc_adc_context_t *)adc_handle;

  // Check that we're in conversion mode.
  if (aioc_adc_context->state != AIOC_ADC_STATE_CONVERSION_MODE) {
    return error_bad_state;
  }

  e = aioc_util_spi_open(aioc_adc_context->spi_dev_id,
                         aioc_adc_context->spi_cs_id);
  if (e) {
    return e;
  }

  // Issue specific input channel selection command.
  e = aioc_adc_conversion_mode_command_channel_selection(adc_input);
  if (e) {
    return e;
  }

  // Pulse the appropriate ADC convert signal active (low) for appropriate
  // duration.
  //  CNV Low Time: tCNVL: 80 ns
  e = aioc_util_ultrascale_gpio_pulse_low(aioc_adc_context->gpio_id, 80);
  if (e) {
    return e;
  }

  // Delay for conversion time.
  //  Conversion Time: tCONVERT: 380-415 ns.
  aioc_util_delay_ns(500);

  // Read the conversion result.
  e = aioc_adc_conversion_mode_result_read(result);
  if (e) {
    return e;
  }

  e = aioc_util_spi_close();
  if (e) {
    return e;
  }

  return error_none;
}

//================================
// Private definitions.
//================================

//==============================================================================
//==============================================================================
static aioc_error_t aioc_adc_reset(i2c_gpio_pin_name_t reset_n) {
  aioc_error_t e = error_none;

  // tRESETL is the minimum amount of time that RESET must be driven low,
  // and tHWR_DELAY is the time that the digital host must wait between a
  // RESET falling edge and starting an SPI frame.
  //
  // RESET Low Time:        tRESETL:    10 ns
  // Hardware Reset Delay:  tHWR_DELAY: 310 µs
  // Software Reset Delay:  tSWR_DELAY: 310 µs

  e = aioc_i2c_gpio_pin_pulse_low(reset_n, 10);
  if (e) {
    return e;
  }

  return error_none;
}

//==============================================================================
//
// Register Configuration Mode routines.
//
//==============================================================================
//==============================================================================
//==============================================================================
static aioc_error_t aioc_adc_self_check(void) {
  aioc_error_t e = error_none;
  uint8_t data;

  // Check the Device Status Register for reset flag or spi error.
  data = 0;
  e = aioc_adc_register_read(STATUS_adrs, &data, 1);
  if (e) {
    return e;
  }
  if (STATUS_RESET_FLAG_read(data) == 0) {
    return error_adc_self_check;
  }
  if (STATUS_SPI_ERROR_read(data)) {
    return error_adc_self_check;
  }

  // Check the Vendor ID of the ADC.
  //  Read vendor id low register and check the id.
  //  Read vendor id high register and check the id.
  e = aioc_adc_register_read(VENDOR_L_adrs, &data, 1);
  if (e) {
    return e;
  }
  if (data != VENDOR_L_reset) {
    return error_adc_self_check;
  }
  data = 0;
  e = aioc_adc_register_read(VENDOR_H_adrs, &data, 1);
  if (e) {
    return e;
  }
  if (data != VENDOR_H_reset) {
    return error_adc_self_check;
  }

  // Check the Device Type of the ADC.
  //  Read device type register and check the device type.
  data = 0;
  e = aioc_adc_register_read(DEVICE_TYPE_adrs, &data, 1);
  if (e) {
    return e;
  }
  if (data != DEVICE_TYPE_reset) {
    return error_adc_self_check;
  }

  // Check scratch pad register access.
  //  Write 0xA5 to scratch pad register then read it back.
  data = 0xA5;
  e = aioc_adc_register_write(SCRATCH_PAD_adrs, &data, 1);
  if (e) {
    return e;
  }
  data = 0;
  e = aioc_adc_register_read(SCRATCH_PAD_adrs, &data, 1);
  if (e) {
    return e;
  }
  if (data != 0xA5) {
    return error_adc_self_check;
  }

  return error_none;
}

//==============================================================================
//==============================================================================
static aioc_error_t aioc_adc_configure_single_cycle_mode(void) {
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
  // settings for each channel are set with the corresponding CONFIG_INn
  // register. Therefore, all analog inputs can have different channel
  // configuration options, analog input high-Z mode enable settings, and
  // threshold detection enable settings. Configure each CONFIG_INn register
  // before entering conversion mode and initiating conversions.

  // Setup channel sequencing for Single-Cycle command mode.
  // Set the STD_SEQ_EN bit to 0, set the NUM_SLOTS_AS bit field to 0x00, and...
  e = aioc_adc_register_mask_write(SEQ_CTRL_adrs,
                                   AD469x_SEQ_CTRL_STD_SEQ_EN_MASK,
                                   AD469x_SEQ_CTRL_STD_SEQ_EN(0));
  if (e) {
    return e;
  }

  e = aioc_adc_register_mask_write(SEQ_CTRL_adrs,
                                   AD469x_SEQ_CTRL_NUM_SLOTS_AS_MASK,
                                   AD469x_SEQ_CTRL_NUM_SLOTS_AS(0));
  if (e) {
    return e;
  }

  // Set the CYC_CTRL bit to 1.
  e = aioc_adc_register_mask_write(SETUP_adrs, AD469x_SETUP_CYC_CTRL_MASK,
                                   AD469x_SETUP_CYC_CTRL_SINGLE(1));
  if (e) {
    return e;
  }

  // Setup the busy signal on GP0.
  //
  // Set the BUSY_GP_EN bit in the GP_MODE register to 1 to enable the
  // busy indicator on the BSY_ALT_GP0 pin.
  // 0: busy indicator on the general-purpose pin function disabled.
  // 1: busy indicator on the general-purpose pin function enabled.
  // GP_MODE reset: 0x0
  //
  // Select BSY_ALT_GP0 pin to configure as the busy indicator with the
  // BUSY_GP_SEL bit in the GP_MODE register.
  // 0: configures BSY_ALT_GP0 as the busy indicator when the BUSY_GP_EN bit
  // is set to 1.
  // 1: configures GP3 as the busy indicator when the BUSY_GP_EN bit
  // is set to 1  data = 0;

  e = aioc_adc_register_mask_write(GP_MODE_adrs, AD469x_GP_MODE_BUSY_GP_EN_MASK,
                                   AD469x_GP_MODE_BUSY_GP_EN(1));
  if (e) {
    return e;
  }

  e = aioc_adc_register_mask_write(GP_MODE_adrs,
                                   AD469x_GP_MODE_BUSY_GP_SEL_MASK,
                                   AD469x_GP_MODE_BUSY_GP_SEL(0));
  if (e) {
    return e;
  }

  return error_none;
}

//==============================================================================
//==============================================================================
static aioc_error_t aioc_adc_conversion_mode_enter(void) {
  aioc_error_t e = error_none;
  uint8_t data = 0;

  // Put the ADC into Conversion mode.
  e = aioc_adc_register_mask_write(SETUP_adrs, AD469x_SETUP_IF_MODE_MASK,
                                   AD469x_SETUP_IF_MODE_CONV);
  if (e) {
    return e;
  }

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
static uint8_t register_buffer[ADC_INSTRUCTION_SIZE + ADC_REGISTER_SIZE_MAX];

//-----------------------------------
// ADC Register read/write routines:
//-----------------------------------
//
// Register Configuration is as such:
//
// We're using (default):
//  Descending Address Direction,
//  MultiByte Register Access, and
//  Streaming Instruction Mode (only one instruction phase is required per
//  SPI frame and the register address being read from or written to is
//  automatically updated after each data phase (based on the selected address
//  direction option)).
// SPI transactions are in 'network (big endian) order': see ADC datasheet
//   Figure 93. Register Configuration Mode Timing Diagrams.
// The ADC registers are little endian; see ADC datasheet, Multibyte Register
// Access:
//   The address of each multibyte register is defined as the address of its
//   least significant byte (LSByte), but the multibyte register contents
//   extend across multiple register addresses. For example, the STD_SEQ_CONFIG
//   register (Address 0x0024) is two bytes long, the address of its LSByte
//   is 0x0024, and the address of its MSByte is 0x0025.
// So for multi-byte register transfers to work:
//   Calculate register address to point at highest byte address of the
//   (possibly multi-byte) register.  Buffer up SOM (little endian) version
//   of register address into big endian (network) order.
//
// The data written or read is in big endian (network) order.  So the caller
//   must deal with the endianness.  For example, one byte registgers have
//   no formatting issue.  Shorts and Longs however do.
//-----------------------------------

static aioc_error_t aioc_adc_register_read(uint32_t register_adrs,
                                           uint8_t *data, uint32_t data_count) {
  aioc_error_t e = error_none;
  const uint16_t read_flag = 0x8000;
  uint16_t instruction = read_flag + ((uint16_t)register_adrs);

  // Check that there is data to read and that the data will fit.
  if ((data_count == 0) ||
      ((sizeof(register_buffer) - ADC_INSTRUCTION_SIZE) < data_count)) {
    return error_bad_param;
  }

  // Clear register buffer.
  memset(register_buffer, 0, sizeof(register_buffer));

  // Calculate register address to point at highest byte address of the
  // (possibly multi-byte) register.
  instruction += (uint16_t)(data_count - 1);

  register_buffer[0] = (uint8_t)((instruction >> 8) & 0x7F); // MSB
  register_buffer[1] = (uint8_t)((instruction)&0xFF);        // LSB

  e = aioc_util_spi_transaction(register_buffer,
                                (data_count + ADC_INSTRUCTION_SIZE));
  if (e) {
    return e;
  }

  memcpy(data, (uint8_t *)(&register_buffer[ADC_INSTRUCTION_SIZE]), data_count);

  return error_none;
}

//==============================================================================
//==============================================================================
static aioc_error_t aioc_adc_register_write(uint32_t register_adrs,
                                            uint8_t *data,
                                            uint32_t data_count) {
  aioc_error_t e = error_none;
  uint16_t instruction = ((uint16_t)register_adrs);

  // Check that there is data to read and that the data will fit.
  if ((data_count == 0) ||
      ((sizeof(register_buffer) - ADC_INSTRUCTION_SIZE) < data_count)) {
    return error_bad_param;
  }

  // Clear register buffer.
  memset(register_buffer, 0, sizeof(register_buffer));

  // Calculate register address to point at highest byte address of the
  // (possibly multi-byte) register.
  instruction += (uint16_t)(data_count - 1);

  register_buffer[0] = (uint8_t)((instruction >> 8) & 0xFF); // MSB
  register_buffer[1] = (uint8_t)((instruction)&0xFF);        // LSB

  memcpy((uint8_t *)(&register_buffer[ADC_INSTRUCTION_SIZE]), data, data_count);

  e = aioc_util_spi_transaction(register_buffer,
                                (data_count + ADC_INSTRUCTION_SIZE));
  if (e) {
    return e;
  }

  return error_none;
}

//==============================================================================
//==============================================================================
static aioc_error_t aioc_adc_register_mask_write(uint32_t register_adrs,
                                                 uint8_t mask, uint8_t data) {
  uint8_t reg_data = 0;
  aioc_error_t e = error_none;

  e = aioc_adc_register_read(register_adrs, &reg_data, 1);
  if (e) {
    return e;
  }

  reg_data &= ~mask;
  reg_data |= data;

  e = aioc_adc_register_write(register_adrs, &reg_data, 1);
  if (e) {
    return e;
  }

  return error_none;
}

//==============================================================================
//
// Conversion Mode routines.
//
//==============================================================================

//==============================================================================
//==============================================================================
// Issue specific input channel selection command.
static aioc_error_t
aioc_adc_conversion_mode_command_channel_selection(uint32_t input) {
  aioc_error_t e = error_none;

  // Issue specific input channel selection command.
  e = aioc_adc_conversion_mode_command_issue(
      CONVERSION_MODE_COMMAND_channel_selection(input));
  if (e) {
    return e;
  }

  return error_none;
}

//==============================================================================
//==============================================================================
static aioc_error_t aioc_adc_conversion_mode_exit(void) {
  aioc_error_t e = error_none;
  uint8_t data = 0;

  // Put the ADC into Register configuration mode.
  // NOTE: This ASSUMES the ADC is in Conversion mode.
  e = aioc_adc_conversion_mode_command_issue(
      CONVERSION_MODE_COMMAND_register_configuration_mode);
  if (e) {
    return e;
  }

  return error_none;
}

//==============================================================================
// Write 5 bit conversion mode command to ADC.
// We're writing out 16 bits.  The command is left adjusted into MSB.
// NOTE: Must be in conversion mode for this to work.
// The way this works:
//  SOM is little endian.
//  SPI frame transactions are big endian.
//  So put that MSB into lowest address.
//==============================================================================
static aioc_error_t aioc_adc_conversion_mode_command_issue(uint32_t command) {
  aioc_error_t e = error_none;
  uint8_t data[2] = {0, 0};

  // Left adjust command
  data[0] = (uint8_t)(command & 0x1F);
  data[0] = data[0] << 3;
  data[1] = 0;

  e = aioc_util_spi_transaction(data, 2);
  if (e) {
    return e;
  }

  return error_none;
}

//==============================================================================
// Read 16 bit result from ADC.
// NOTE: Must be in conversion mode for this to work.
//==============================================================================
static aioc_error_t aioc_adc_conversion_mode_result_read(uint16_t *result) {
  aioc_error_t e = error_none;
  uint8_t data[2] = {0, 0};

  // Read 16 bits.
  e = aioc_util_spi_transaction(data, 2);
  if (e) {
    return e;
  }

  // Transaction put data in big endian, this is little endian machine.
  // So act accordingly.
  *result = (uint16_t)(((((uint16_t)(data[0])) << 8) & 0xFF00) |
                       ((uint16_t)(data[1]) & 0x00FF));

  return error_none;
}

// TBD are conversion mode txact just 2 or are they 3 bytes long.