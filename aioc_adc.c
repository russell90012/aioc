// MERCURY HEADER GOES HERE
// TBD

#include "aioc_adc.h"
#include "aioc_i2c_gpio.h"
#include "aioc_util.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// TBD are conversion mode txact just 2 or are they 3 bytes long.


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
#define AD469x_SETUP_SPI_MODE_MASK (0x01 << 2)
#define AD469x_SETUP_SPI_MODE_CONV (0x01 << 2)
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

//==============================================================================
//==============================================================================

/**
 * Perform a hard reset on the ADC.
 *
 * @param reset_n is the i2c_gpio reset line.
 *
 * @return error handling result code.
 */
static aioc_error_t aioc_adc_reset_dev(struct aioc_adc_dev* dev);

/**
 * Check vendor and device id register values for expected values.  Check that
 * the scratch pad register works as expected.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
static aioc_error_t aioc_adc_self_check(struct aioc_adc_dev* dev);

/**
 * Program the channel sequencing to for single-cycle mode.  And allocate
 * the busy (converting) signal to the ADC's GP0.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
static aioc_error_t aioc_adc_configure_single_cycle_mode(struct aioc_adc_dev* dev);

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
static aioc_error_t aioc_adc_register_read(struct aioc_adc_dev* dev,
  uint32_t register_adrs,
  uint8_t *data,
  uint32_t data_count);

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
static aioc_error_t aioc_adc_register_write(struct aioc_adc_dev* dev,
  uint32_t register_adrs,
  uint8_t *data,
  uint32_t data_count);

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
static aioc_error_t aioc_adc_register_mask_write(struct aioc_adc_dev* dev,
  uint32_t register_adrs,
  uint8_t mask,
  uint8_t data);

/**
 * Enter the ADC into Conversion mode.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
static aioc_error_t aioc_adc_conversion_mode_enter(struct aioc_adc_dev* dev);

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
aioc_adc_conversion_mode_command_channel_selection(struct aioc_adc_dev* dev, uint32_t input);

/**
 * Exit the ADC from Conversion Mode.
 *
 * @return error handling result code.
 *
 * @note the ADC we're using here is specifed by the SPI device.  And
 *       that device was opened using the ADC context info.
 */
static aioc_error_t aioc_adc_conversion_mode_exit(struct aioc_adc_dev* dev);

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
static aioc_error_t aioc_adc_conversion_mode_command_write(struct aioc_adc_dev* dev, uint32_t command);

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
static aioc_error_t aioc_adc_conversion_mode_result_read(struct aioc_adc_dev* dev, uint16_t *result);



//================================
// Public  definitions.
//================================

//==============================================================================
//==============================================================================
aioc_error_t 
aioc_adc_init(
  struct ad469x_dev** device,
  struct ad469x_init_param* init_param)
{
  struct ad469x_dev* dev;
  aioc_error_t e = error_none;
#ifdef  AIOC_DEBUG
printf("%s: enter\n", __FUNCTION__);
#endif
 
	// TBD
  dev = (struct aioc_adc_dev *)malloc(sizeof(*dev));
	if (!dev)
  {
    return error_alloc;
  }

	e = aioc_util_spi_init(&dev->spi_desc, init_param->spi_init);
  if (e) {  return e;  }

	e = ad469x_init_gpio(dev, init_param);
  if (e) {  return e;  }

// Perform ADC reset.
  e = aioc_adc_reset_dev(dev);
  if (e) {  return e;  }

  // Perform ADC self-check.
  e = aioc_adc_self_check(dev);
  if (e) {  return e;  }

  // Since we passed self check, setup internal state
  // as register configuration mode.
  dev->state = AIOC_ADC_STATE_REGISTER_MODE;

  // Configure the ADC
  e = aioc_adc_configure_single_cycle_mode(dev);
  if (e) {  return e;  }

  // Start conversion mode.
  e = aioc_adc_conversion_mode_enter(dev);
  if (e) {  return e;  }

  // Since we passed configuration and ADC entered conversion mode,
  // setup internal state to conversion mode.
  // And pass back context pointer as a handle.
  dev->state = AIOC_ADC_STATE_CONVERSION_MODE;
  *device = dev;

  
#ifdef  AIOC_DEBUG
printf("%s: exit\n", __FUNCTION__);
#endif
   return error_none;
}

//==============================================================================
//==============================================================================
aioc_error_t
aioc_adc_convert_single_cycle(
  struct aioc_adc_dev* dev,
  aioc_adc_input_t adc_input,
  uint16_t *result)
{
  aioc_error_t e = error_none;
#ifdef  AIOC_DEBUG
printf("%s: enter\n", __FUNCTION__);
#endif

  // Check that we're in conversion mode.
  if (dev->state != AIOC_ADC_STATE_CONVERSION_MODE)
  {
    return error_bad_state;
  }

  // Issue specific input channel selection command.
  e = aioc_adc_conversion_mode_command_channel_selection(dev, adc_input);
  if (e) {  return e;  }

  // Pulse the appropriate ADC convert signal active (low) for appropriate
  // duration.
  //  CNV Low Time: tCNVL: 80 ns
  e = aioc_util_ultrascale_gpio_pulse_low(dev, 80);
  if (e) {  return e;  }

  // Delay for conversion time.
  //  Conversion Time: tCONVERT: 380-415 ns.
  aioc_util_delay_ns(500);

  // Read the conversion result.
  e = aioc_adc_conversion_mode_result_read(dev, result);
  if (e) {  return e;  }

#ifdef  AIOC_DEBUG
printf("%s: exit\n", __FUNCTION__);
#endif
  return error_none;
}



//================================
// Private definitions.
//================================

//==============================================================================
//==============================================================================
static aioc_error_t aioc_adc_reset_dev(struct ad469x_dev* dev)
{
  aioc_error_t e = error_none;

  // tRESETL is the minimum amount of time that RESET must be driven low,
  // and tHWR_DELAY is the time that the digital host must wait between a
  // RESET falling edge and starting an SPI frame.
  //
  // RESET Low Time:        tRESETL:    10 ns
  // Hardware Reset Delay:  tHWR_DELAY: 310 µs
  // Software Reset Delay:  tSWR_DELAY: 310 µs

  e = aioc_i2c_gpio_pin_pulse_low(reset_n, 10);
  if (e) {  return e;  }

  return error_none;
}

//==============================================================================
//
// Register Configuration Mode routines.
//
//==============================================================================
/**
 * @brief Initialize GPIO driver handlers for the GPIOs in the system.
 *        ad469x_init() helper function.
 * @param [out] dev - ad469x_dev device handler.
 * @param [in] init_param - Pointer to the initialization structure.
 * @return 0 in case of success, -1 otherwise.
 */
static int32_t ad469x_init_gpio(
         struct ad469x_dev *dev,
				 struct ad469x_init_param *init_param)
{
	int32_t ret;

	ret = no_os_gpio_get_optional(&dev->gpio_resetn, init_param->gpio_resetn);
	if (ret != 0)
		return ret;

	ret = no_os_gpio_direction_output(dev->gpio_resetn, NO_OS_GPIO_HIGH);
	if (ret != 0)
		return ret;


	return 0;
}






//==============================================================================
//==============================================================================
static aioc_error_t
aioc_adc_self_check(struct aioc_adc_dev* dev)
{
  aioc_error_t e = error_none;
  uint8_t data;
#ifdef  AIOC_DEBUG
printf("%s: enter\n", __FUNCTION__);
#endif

  // Check the Device Status Register for reset flag or spi error.
  data = 0;
  e = aioc_adc_register_read(dev, STATUS_adrs, &data, 1);
  if (e) {  return e;  }
  if (STATUS_RESET_FLAG_read(data) == 0)
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
  e = aioc_adc_register_read(dev, VENDOR_L_adrs, &data, 1);
  if (e) {  return e;  }
  if (data != VENDOR_L_reset)
  {
    return error_adc_self_check;
  }
  data = 0;
  e = aioc_adc_register_read(dev, VENDOR_H_adrs, &data, 1);
  if (e) {  return e;  }
  if (data != VENDOR_H_reset)
  {
    return error_adc_self_check;
  }

  // Check the Device Type of the ADC.
  //  Read device type register and check the device type.
  data = 0;
  e = aioc_adc_register_read(dev, DEVICE_TYPE_adrs, &data, 1);
  if (e) {  return e;  }
  if (data != DEVICE_TYPE_reset)
  {
    return error_adc_self_check;
  }

  // Check scratch pad register access.
  //  Write 0xA5 to scratch pad register then read it back.
  data = 0xA5;
  e = aioc_adc_register_write(dev, SCRATCH_PAD_adrs, &data, 1);
  if (e) {  return e;  }
  data = 0;
  e = aioc_adc_register_read(dev, SCRATCH_PAD_adrs, &data, 1);
  if (e) {  return e;  }
  if (data != 0xA5)
  {
    return error_adc_self_check;
  }

#ifdef  AIOC_DEBUG
printf("%s: exit\n", __FUNCTION__);
#endif
  return error_none;
}

//==============================================================================
//==============================================================================
static aioc_error_t
aioc_adc_configure_single_cycle_mode(struct aioc_adc_dev* dev)
{
  aioc_error_t e = error_none;
  uint8_t data;
#ifdef  AIOC_DEBUG
printf("%s: enter\n", __FUNCTION__);
#endif

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
  e = aioc_adc_register_mask_write(dev, 
        SEQ_CTRL_adrs,
        AD469x_SEQ_CTRL_STD_SEQ_EN_MASK,
        AD469x_SEQ_CTRL_STD_SEQ_EN(0));
  if (e) {  return e;  }

  e = aioc_adc_register_mask_write(dev, 
        SEQ_CTRL_adrs,
        AD469x_SEQ_CTRL_NUM_SLOTS_AS_MASK,
        AD469x_SEQ_CTRL_NUM_SLOTS_AS(0));
  if (e) {  return e;  }

  // Set the CYC_CTRL bit to 1.
  e = aioc_adc_register_mask_write(dev, 
        SETUP_adrs,
        AD469x_SETUP_CYC_CTRL_MASK,
        AD469x_SETUP_CYC_CTRL_SINGLE(1));
  if (e) {  return e;  }

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

  e = aioc_adc_register_mask_write(dev, 
        GP_MODE_adrs,
        AD469x_GP_MODE_BUSY_GP_EN_MASK,
        AD469x_GP_MODE_BUSY_GP_EN(1));
  if (e) {  return e;  }

  e = aioc_adc_register_mask_write(dev, 
        GP_MODE_adrs,
        AD469x_GP_MODE_BUSY_GP_SEL_MASK,
        AD469x_GP_MODE_BUSY_GP_SEL(0));
  if (e) {  return e;  }

#ifdef  AIOC_DEBUG
printf("%s: exit\n", __FUNCTION__);
#endif
  return error_none;
}

//==============================================================================
//==============================================================================
static aioc_error_t
aioc_adc_conversion_mode_enter(struct aioc_adc_dev* dev)
{
  aioc_error_t e = error_none;
  uint8_t data = 0;

  // Put the ADC into Conversion mode.
  e = aioc_adc_register_mask_write(dev, 
        SETUP_adrs,
        AD469x_SETUP_SPI_MODE_MASK,
        AD469x_SETUP_SPI_MODE_CONV);
  if (e) {  return e;  }

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

static aioc_error_t
aioc_adc_register_read(struct aioc_adc_dev* dev,
  uint32_t register_adrs,
  uint8_t *data,
  uint32_t data_count)
{
  aioc_error_t e = error_none;
  const uint16_t read_flag = 0x8000;
  uint16_t instruction = read_flag + ((uint16_t)register_adrs);

  // Check that there is data to read and that the data will fit.
  if ((data_count == 0) ||
      ((sizeof(register_buffer) - ADC_INSTRUCTION_SIZE) < data_count))
  {
    return error_bad_param;
  }

  // Clear register buffer.
  memset(register_buffer, 0, sizeof(register_buffer));

  // Calculate register address to point at highest byte address of the
  // (possibly multi-byte) register.
  instruction += (uint16_t)(data_count - 1);

  register_buffer[0] = (uint8_t)((instruction >> 8) & 0xFF); // MSB
  register_buffer[1] = (uint8_t)((instruction)&0xFF);        // LSB

  e = aioc_util_spi_transaction(dev->spi_desc,
        register_buffer,
        (data_count + ADC_INSTRUCTION_SIZE));
  if (e) {  return e;  }

  memcpy(data, (uint8_t *)(&register_buffer[ADC_INSTRUCTION_SIZE]), data_count);

  return error_none;
}

//==============================================================================
//==============================================================================
static aioc_error_t
aioc_adc_register_write(struct aioc_adc_dev* dev,
  uint32_t register_adrs,
  uint8_t *data,
  uint32_t data_count)
{
  aioc_error_t e = error_none;
  uint16_t instruction = ((uint16_t)register_adrs);

  // Check that there is data to read and that the data will fit.
  if ((data_count == 0) ||
      ((sizeof(register_buffer) - ADC_INSTRUCTION_SIZE) < data_count))
  {
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

  e = aioc_util_spi_transaction(dev->spi_desc, 
        register_buffer,
        (data_count + ADC_INSTRUCTION_SIZE));
  if (e) {  return e;  }

  return error_none;
}

//==============================================================================
//==============================================================================
static aioc_error_t
aioc_adc_register_mask_write(struct aioc_adc_dev* dev,
  uint32_t register_adrs,
  uint8_t mask,
  uint8_t data)
{
  uint8_t reg_data = 0;
  aioc_error_t e = error_none;

  e = aioc_adc_register_read(dev, register_adrs, &reg_data, 1);
  if (e) {  return e;  }

  reg_data &= ~mask;
  reg_data |= data;

  e = aioc_adc_register_write(dev, register_adrs, &reg_data, 1);
  if (e) {  return e;  }

  return error_none;
}

//==============================================================================
//
// Conversion Mode routines.
//
//==============================================================================

//==============================================================================
// Issue specific input channel selection command.
//==============================================================================
static aioc_error_t
aioc_adc_conversion_mode_command_channel_selection(struct aioc_adc_dev* dev, uint32_t input)
{
  aioc_error_t e = error_none;

  // Issue specific input channel selection command.
  e = aioc_adc_conversion_mode_command_write(dev, (0x10 + (input & 0xF)));
  if (e) {  return e;  }

  return error_none;
}

//==============================================================================
//==============================================================================
static aioc_error_t
aioc_adc_conversion_mode_exit(struct aioc_adc_dev* dev)
{
  aioc_error_t e = error_none;
  uint8_t data = 0;

  // Put the ADC into Register configuration mode.
  // NOTE: This ASSUMES the ADC is in Conversion mode.
  e = aioc_adc_conversion_mode_command_write(dev, 0x0A);
  if (e) {  return e;  }

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
static aioc_error_t
aioc_adc_conversion_mode_command_write(struct aioc_adc_dev* dev, uint32_t command)
{
  aioc_error_t e = error_none;
  uint8_t data[2] = {0, 0};

  // Left adjust command
  data[0] = (uint8_t)(((command & 0x1F) << 3) & 0xFF);
  data[1] = 0;

  e = aioc_util_spi_transaction(dev->spi_desc, data, 2);
  if (e) {  return e;  }

  return error_none;
}

//==============================================================================
// Read 16 bit result from ADC.
// NOTE: Must be in conversion mode for this to work.
//==============================================================================
static aioc_error_t
aioc_adc_conversion_mode_result_read(struct aioc_adc_dev* dev, uint16_t *result)
{
  aioc_error_t e = error_none;
  uint8_t data[2] = {0, 0};

  // Read 16 bits.
  e = aioc_util_spi_transaction(dev->spi_desc, data, 2);
  if (e) {  return e;  }

  // Transaction put data in big endian, this is little endian machine.
  // So act accordingly.
  *result = (uint16_t)(((
                ((uint16_t)(data[0])) << 8) & 0xFF00) |
                ((uint16_t)(data[1]) & 0x00FF));

  return error_none;
}

