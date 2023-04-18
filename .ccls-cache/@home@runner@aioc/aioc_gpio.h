// MERCURY HEADER GOES HERE
// TBD

/**
 * @file:    aioc_gpio.h
 * @author:  Russell Shahenian
 * @since:   4/17/23
 * @brief:   This file contains the public AIOC gpio module specifications.
 */

#include "aioc_defs.h"


//================================
// Public specifications.
//================================*****************/

#ifndef _NO_OS_GPIO_H_
#define _NO_OS_GPIO_H_

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/

#include <stdint.h>

/******************************************************************************/
/********************** Macros and Constants Definitions **********************/
/******************************************************************************/

#define NO_OS_GPIO_OUT	0x01
#define NO_OS_GPIO_IN		0x00

/******************************************************************************/
/*************************** Types Declarations *******************************/
/******************************************************************************/
enum xil_gpio_type {
	/** Processing System */
	GPIO_PS,
  /** I2C GPIO expander */
	GPIO_I2C
};

/**
 * @struct xil_gpio_init_param
 * @brief Structure holding the initialization parameters for Xilinx platform
 * specific GPIO parameters.
 */
struct xil_gpio_init_param {
	/** Xilinx architecture */
	enum xil_gpio_type	type;
	/** Device ID */
	uint32_t		device_id;
};

/**
 * @struct xil_gpio_desc
 * @brief Xilinx platform specific GPIO descriptor
 */
struct xil_gpio_desc {
	/** Xilinx architecture */
	enum xil_gpio_type	type;
	/** Xilinx GPIO configuration */
	void			*config;
	/** Xilinx GPIO Instance */
	void			*instance;
};


/**
 * @struct no_os_gpio_platform_ops
 * @brief Structure holding gpio function pointers that point to the platform
 * specific function
 */
struct no_os_gpio_platform_ops ;

/**
 * @enum no_os_gpio_pull_up
 * @brief Enum that holds the possible pull up/ pull down resistor configuration.
 */
enum no_os_gpio_pull_up {
	NO_OS_PULL_NONE,
	/** Strong pull up */
	NO_OS_PULL_UP,
	/** Strong pull down */
	NO_OS_PULL_DOWN,
	NO_OS_PULL_UP_WEAK,
	NO_OS_PULL_DOWN_WEAK
};

/**
 * @struct no_os_gpio_init_param
 * @brief Structure holding the parameters for GPIO initialization.
 */
struct no_os_gpio_init_param {
	/** Port number */
	int32_t		port;
	/** GPIO number */
	int32_t		number;
	/** Pull up/down resistor configuration */
	enum no_os_gpio_pull_up pull;
	/** GPIO platform specific functions */
	const struct no_os_gpio_platform_ops *platform_ops;
	/** GPIO extra parameters (device specific) */
	void		*extra;
};

/**
 * @struct no_os_gpio_desc
 * @brief Structure holding the GPIO descriptor.
 */
struct no_os_gpio_desc {
	/** Port number */
	int32_t		port;
	/** GPIO number */
	int32_t		number;
	/** Pull up/down resistor configuration */
	enum no_os_gpio_pull_up pull;
	/** GPIO platform specific functions */
	const struct no_os_gpio_platform_ops *platform_ops;
	/** GPIO extra parameters (device specific) */
	void		*extra;
};

/**
 * @enum no_os_gpio_values
 * @brief Enum that holds the possible output states of a GPIO.
 */
enum no_os_gpio_values {
	/** GPIO logic low */
	NO_OS_GPIO_LOW,
	/** GPIO logic high */
	NO_OS_GPIO_HIGH,
	/** GPIO high impedance */
	NO_OS_GPIO_HIGH_Z
};

/******************************************************************************/
/************************ Functions Declarations ******************************/
/******************************************************************************/

/* Obtain the GPIO decriptor. */
int32_t no_os_gpio_get(struct no_os_gpio_desc **desc,
		       const struct no_os_gpio_init_param *param);

/* Obtain optional GPIO descriptor. */
int32_t no_os_gpio_get_optional(struct no_os_gpio_desc **desc,
				const struct no_os_gpio_init_param *param);

/* Free the resources allocated by no_os_gpio_get(). */
int32_t no_os_gpio_remove(struct no_os_gpio_desc *desc);

/* Enable the input direction of the specified GPIO. */
int32_t no_os_gpio_direction_input(struct no_os_gpio_desc *desc);

/* Enable the output direction of the specified GPIO. */
int32_t no_os_gpio_direction_output(struct no_os_gpio_desc *desc,
				    uint8_t value);

/* Get the direction of the specified GPIO. */
int32_t no_os_gpio_get_direction(struct no_os_gpio_desc *desc,
				 uint8_t *direction);

/* Set the value of the specified GPIO. */
int32_t no_os_gpio_set_value(struct no_os_gpio_desc *desc,
			     uint8_t value);

/* Get the value of the specified GPIO. */
int32_t no_os_gpio_get_value(struct no_os_gpio_desc *desc,
			     uint8_t *value);

#endif // _NO_OS_GPIO_H_
