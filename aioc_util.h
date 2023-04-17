#ifndef AIOC_UTIL_H
#define AIOC_UTIL_H

#include "aioc_defs.h"

//
//
//

aioc_error_t
aioc_util_delay_ns(uint32_t delay);




aioc_error_t
aioc_util_ultrascale_gpio_pulse_low(
  uint32_t convert_id, 
  uint32_t pulse_duration);


//
// SPI
//
/**
 * @enum no_os_spi_bit_order
 * @brief SPI configuration for bit order (MSB/LSB).
 */
enum no_os_spi_bit_order {
	/** Most-significant bit (MSB) first */
	NO_OS_SPI_BIT_ORDER_MSB_FIRST = 0,
	/** Least-significant bit (LSB) first */
	NO_OS_SPI_BIT_ORDER_LSB_FIRST = 1,
};

/**
 * @struct no_os_spi_init_param
 * @brief Structure holding the parameters for SPI initialization
 */
struct no_os_spi_init_param {
	/** Device ID */
	uint32_t	device_id;
	/** maximum transfer speed */
	uint32_t	max_speed_hz;
	/** SPI chip select */
	uint8_t		chip_select;
	/** SPI bit order */
	enum no_os_spi_bit_order	bit_order;
	/**  SPI extra parameters (device specific) */
	void		*extra;
	/** Parent of the device */
	struct no_os_spi_desc *parent;
};

/**
 * @struct no_os_spi_desc
 * @brief Structure holding SPI descriptor.
 */
struct no_os_spi_desc {
	/** Device ID */
	uint32_t	device_id;
	/** maximum transfer speed */
	uint32_t	max_speed_hz;
	/** SPI chip select */
	uint8_t		chip_select;
	/** SPI bit order */
	enum no_os_spi_bit_order	bit_order;
	/**  SPI extra parameters (device specific) */
	void		*extra;
	/** Parent of the device */
	struct no_os_spi_desc *parent;
};
aioc_error_t aioc_util_spi_init(
              struct no_os_spi_desc ** spi_desc,
              struct no_os_spi_init_param* parm);


aioc_error_t aioc_util_spi_transaction(
              struct no_os_spi_init_param * spi_desc,
              uint8_t* data,
              uint32_t number_of_bytes);

//
// I2C
//
/**
 * @struct no_os_i2c_init_param
 * @brief Structure holding the parameters for I2C initialization.
 */
struct no_os_i2c_init_param {
	/** Device ID */
	uint32_t	device_id;
	/** I2C maximum transfer speed supported */
	uint32_t	max_speed_hz;
	/** Slave address */
	uint8_t		slave_address;
	/** I2C platform specific functions */
	const struct no_os_i2c_platform_ops *platform_ops;
	/** I2C extra parameters (device specific parameters) */
	void		*extra;
};

/**
 * @struct no_os_i2c_desc
 * @brief Structure holding I2C descriptor
 */
struct no_os_i2c_desc {
	/** Device ID */
	uint32_t	device_id;
	/** I2C maximum transfer speed supported */
	uint32_t	max_speed_hz;
	/** Slave address */
	uint8_t		slave_address;
	/** I2C platform specific functions */
	const struct no_os_i2c_platform_ops *platform_ops;
	/** I2C extra parameters (device specific parameters) */
	void		*extra;
};

{
aioc_error_t aioc_util_i2c_init(struct no_os_i2c_desc ** i2c_desc, no_os_i2c_init_param *param);

aioc_error_t aioc_util_i2c_write(no_os_i2c_desc* desc,
  uint32_t dev_adrs,
  uint32_t reg_adrs,
  uint8_t* data,
  uint32_t num_bytes);




#endif  // AIOC_UTIL_H
