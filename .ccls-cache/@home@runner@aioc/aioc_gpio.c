/***************************************************************************//**
 *   @file   no_os_gpio.c
 *   @brief  Implementation of the GPIO Interface
 *   @author Antoniu Miclaus (antoniu.miclaus@analog.com)
********************************************************************************
 * Copyright 2020(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "aioc_gpio.h"
#include "aioc_defs.h"
#include <inttypes.h>
#include <stdlib.h>

/******************************************************************************/
/************************ Functions Definitions *******************************/
/******************************************************************************/
/**
 * @brief Prepare the GPIO decriptor.
 * @param desc - The GPIO descriptor.
 * @param param - The structure that contains the GPIO parameters.
 * @return 0 in case of success, -1 otherwise.
 */
int32_t _gpio_init(struct no_os_gpio_desc *desc,
		   const struct no_os_gpio_init_param *param)
{
	int32_t				ret;
	struct xil_gpio_desc		*xdesc;
	struct xil_gpio_init_param	*xinit;

	ret = -1;

	xdesc = desc->extra;
	xinit = param->extra;

	xdesc->type = xinit->type;
	desc->number = param->number;

	switch (xinit->type) {
	case GPIO_I2C:
#ifdef XGPIO_H
		xdesc->instance = (XGpio *)malloc(sizeof(XGpio));
		if(!xdesc->instance)
			goto pl_error;

		xdesc->config = XGpio_LookupConfig(xinit->device_id);
		if(xdesc->config == NULL)
			goto pl_error;

		ret = XGpio_CfgInitialize(xdesc->instance,
					  xdesc->config,
					  ((XGpio_Config*)xdesc->config)
					  ->BaseAddress);
		if(ret < 0)
			goto pl_error;

		break;
pl_error:
		free(xdesc->instance);
#endif
		goto error;
	case GPIO_PS:
#ifdef XGPIOPS_H
		xdesc->instance = (XGpioPs *)malloc(sizeof(XGpioPs));
		if(!xdesc->instance)
			goto ps_error;

		xdesc->config = XGpioPs_LookupConfig(xinit->device_id);
		if(xdesc->config == NULL)
			goto ps_error;

		ret = XGpioPs_CfgInitialize(xdesc->instance,
					    xdesc->config,
					    ((XGpioPs_Config*)xdesc->config)->BaseAddr);
		if(ret < 0)
			goto ps_error;
		break;
ps_error:
		free(xdesc->instance);
#endif
		goto error;
error:
	default:
		return -1;
	}

	return 0;
}


/**
 * @brief Obtain the GPIO decriptor.
 * @param desc - The GPIO descriptor.
 * @param param - GPIO Initialization parameters.
 * @return 0 in case of success, -1 otherwise.
 */
int32_t no_os_gpio_get(struct no_os_gpio_desc **desc,
		       const struct no_os_gpio_init_param *param)
{
	struct no_os_gpio_desc	*descriptor;
	struct xil_gpio_desc	*extra;
	int32_t			ret;

	descriptor = (struct no_os_gpio_desc *)malloc(sizeof(*descriptor));
	extra = (struct xil_gpio_desc*)malloc(sizeof(*extra));

	if (!descriptor || !extra)
		return -1;

	descriptor->extra = extra;
	ret = _gpio_init(descriptor, param);

	if(ret != 0)
		goto error;

	*desc = descriptor;

	return 0;
error:
	free(extra);
	free(descriptor);

	return -1;
}



/**
 * @brief Get the value of an optional GPIO.
 * @param desc - The GPIO descriptor.
 * @param param - GPIO Initialization parameters.
 * @return 0 in case of success, -1 otherwise.
 */
int32_t no_os_gpio_get_optional(struct no_os_gpio_desc **desc,
				const struct no_os_gpio_init_param *param)
{
	int32_t ret;

	if (!param || (param->number == -1)) {
		*desc = NULL;
		return 0;
	}

 	no_os_gpio_get(desc, param);

	
	return 0;
}
/**
 * @brief Free the resources allocated by no_os_gpio_get().
 * @param desc - The GPIO descriptor.
 * @return 0 in case of success, -1 otherwise.
 */
int32_t no_os_gpio_remove(struct no_os_gpio_desc *desc)
{
	
	return 0;
}

/**
 * @brief Enable the input direction of the specified GPIO.
 * @param desc - The GPIO descriptor.
 * @return 0 in case of success, -1 otherwise.
 */
int32_t no_os_gpio_direction_input(struct no_os_gpio_desc *desc)
{
	if (desc)
  {
	}

  	return 0;
}

/**
 * @brief Enable the output direction of the specified GPIO.
 * @param desc - The GPIO descriptor.
 * @param value - The value.
 *                Example: NO_OS_GPIO_HIGH
 *                         NO_OS_GPIO_LOW
 * @return 0 in case of success, -1 otherwise.
 */
int32_t no_os_gpio_direction_output(struct no_os_gpio_desc *desc,
				    uint8_t value)
{
	if (desc)
  {
	}

	return 0;
}

/**
 * @brief Get the direction of the specified GPIO.
 * @param desc - The GPIO descriptor.
 * @param direction - The direction.
 *                    Example: NO_OS_GPIO_OUT
 *                             NO_OS_GPIO_IN
 * @return 0 in case of success, -1 otherwise.
 */
int32_t no_os_gpio_get_direction(struct no_os_gpio_desc *desc,
				 uint8_t *direction)
{
	if (desc)
  {
	}

	return 0;
}

/**
 * @brief Set the value of the specified GPIO.
 * @param desc - The GPIO descriptor.
 * @param value - The value.
 *                Example: NO_OS_GPIO_HIGH
 *                         NO_OS_GPIO_LOW
 * @return 0 in case of success, -1 otherwise.
 */
int32_t no_os_gpio_set_value(struct no_os_gpio_desc *desc,
			     uint8_t value)
{
	if (desc)
  {
	}

	return 0;
}

/**
 * @brief Get the value of the specified GPIO.
 * @param desc - The GPIO descriptor.
 * @param value - The value.
 *                Example: NO_OS_GPIO_HIGH
 *                         NO_OS_GPIO_LOW
 * @return 0 in case of success, -1 otherwise.
 */
int32_t no_os_gpio_get_value(struct no_os_gpio_desc *desc,
			     uint8_t *value)
{
	if (desc)
  {
	}

	return 0;
}
