#ifndef  AIOC_ADC_H
#define AIOC_ADC_H

// MERCURY HEADER GOES HERE
// TBD

/**
 * @file:    aioc_adc.h
 * @author:  Russell Shahenian
 * @since:   4/4/23
 * @brief:   This file contains the public AIOC adc module specifications.
 */

#include "aioc_defs.h"
#include "aioc_i2c_gpio.h"
#include "aioc_util.h"


//================================
// Public specifications.
//================================

typedef enum
{
  AIOC_ADC_ID_5V,
  AIOC_ADC_ID_7V,
  AIOC_ADC_ID_95mV,
  AIOC_ADC_ID_RTD,
  AIOC_ADC_ID_EP10,
  
  NUMBER_OF_AIOC_ADC_IDS
}
aioc_adc_id_t;

typedef enum
{
  // AI Analog 0-5 VDC
  A5V_00_MON = 0,
  A5V_01_MON,
  A5V_02_MON,
  A5V_03_MON,
  A5V_04_MON,
  A5V_05_MON,
  A5V_06_MON,
  A5V_07_MON,
  A5V_08_MON,
  A5V_09_MON,
  A5V_10_MON,
  A5V_11_MON,
  A5V_12_MON
}
aioc_adc_input_t;

typedef void* aioc_adc_handle_t;

typedef enum
{
  AIOC_ADC_STATE_RESET = 0,
  AIOC_ADC_STATE_REGISTER_MODE = 1,
  AIOC_ADC_STATE_CONVERSION_MODE
}
aioc_adc_state_t;



/**
 * @struct ad469x_init_param
 * @brief  Structure containing the init parameters needed by the ad469x device
 */
struct ad469x_init_param {
	/* SPI */
	struct no_os_spi_init_param		*spi_init;
#if !defined(USE_STANDARD_SPI)
	/* SPI module offload init */
	struct spi_engine_offload_init_param *offload_init_param;
	/* PWM generator init structure */
	struct no_os_pwm_init_param	*trigger_pwm_init;
	/* Clock gen for hdl design init structure */
	struct axi_clkgen_init	*clkgen_init;
	/* Clock generator rate */
	uint32_t		axi_clkgen_rate;
#endif
	/** RESET GPIO initialization structure. */
	struct no_os_gpio_init_param	*gpio_resetn;
	/** CONVST GPIO initialization parameters */
	struct no_os_gpio_init_param *gpio_convst;
	/** BUSY GPIO initialization parameters */
	struct no_os_gpio_init_param *gpio_busy;
	/* Register access speed */
	uint32_t		reg_access_speed;
	/* Register data width */
	uint8_t		reg_data_width;
	/* Capture data width */
	uint8_t		capture_data_width;
	/* Device Settings */
	enum ad469x_supported_dev_ids dev_id;
	/* Pin Pairing option in standard sequencer mode */
	enum ad469x_pin_pairing std_seq_pin_pairing;
	/* Channel sequencing mode */
	enum ad469x_channel_sequencing ch_sequence;
	/** OSR resolution corresponding to all channels, when standard
	 * sequencer is selected. */
	enum ad469x_osr_ratios std_seq_osr;
	/** OSR resolution corresponding to each channel, when advanced
	 * sequencer is selected. */
	enum ad469x_osr_ratios adv_seq_osr_resol[AD469x_CHANNEL_NO];
	/** Invalidate the Data cache for the given address range */
	void (*dcache_invalidate_range)(uint32_t address, uint32_t bytes_count);
};

/**
 * @struct ad469x_dev
 * @brief  Structure representing an ad469x device
 */
struct ad469x_dev {
	/* SPI descriptor */
	struct no_os_spi_desc		*spi_desc;
#if !defined(USE_STANDARD_SPI)
	/* Clock gen for hdl design structure */
	struct axi_clkgen	*clkgen;
	/* Trigger conversion PWM generator descriptor */
	struct no_os_pwm_desc		*trigger_pwm_desc;
	/* SPI module offload init */
	struct spi_engine_offload_init_param *offload_init_param;
#endif
	/* Register access speed */
	uint32_t		reg_access_speed;
	/* Register data width */
	uint8_t		reg_data_width;
	/* Capture data width */
	uint8_t		capture_data_width;
	/* Device Settings */
	enum ad469x_supported_dev_ids dev_id;
	/** RESET GPIO handler. */
	struct no_os_gpio_desc	*gpio_resetn;
	/** CONVST GPIO descriptor */
	struct no_os_gpio_desc *gpio_convst;
	/** BUSY GPIO descriptor */
	struct no_os_gpio_desc *gpio_busy;
	/** Invalidate the Data cache for the given address range */
	void (*dcache_invalidate_range)(uint32_t address, uint32_t bytes_count);
	/** Current channel sequence */
	enum ad469x_channel_sequencing ch_sequence;
	/* Pin Pairing option in standard sequencer mode */
	enum ad469x_pin_pairing std_seq_pin_pairing;
	/** OSR resolution corresponding to all channels, when standard
	 * sequencer is selected. */
	enum ad469x_osr_ratios std_seq_osr;
	/** OSR resolution corresponding to each channel, when advanced
	 * sequencer is selected. */
	enum ad469x_osr_ratios adv_seq_osr_resol[AD469x_CHANNEL_NO];
	/** Channel slots for advanced sequencer */
	uint8_t ch_slots[AD469x_SLOTS_NO];
	/** Temperature enabled for standard and advanced sequencer if set. */
	bool temp_enabled;
	/** Number of active channel slots, for advanced sequencer */
	uint8_t num_slots;
};








struct aioc_adc_parm_init
{
  struct aioc_spi_parm_init*        spi_parm_init;
	struct aioc_i2c_gpio_parm_init*   gpio_parm_init_reset_n;  
	struct aioc_i2c_gpio_parm_init*   gpio_parm_init_busy;  
	struct aioc_i2c_gpio_parm_init*   gpio_parm_init_convert;
};


struct aioc_adc_dev
{
	/* SPI descriptor */
	struct aioc_util_spi_descriptor		*spi_desc;
  aioc_adc_state_t state;
};



/**
 * Perform ADC self-check, configuration, and start conversion mode for
 * this ADC.  And return handle to this ADC.
 *
 * @param dev is the aioc device context that is intialized passed back to
 *        the caller.
 *
 * @param adc_parm_init is the ADC parameter structure that helps
 *         to initialize the ADC device.
 *
 * @return error handling result code.
 */
aioc_error_t aioc_adc_init(
  struct aioc_adc_dev** device,
  struct aioc_adc_parm_init* adc_parm_init);

/**
 * Perform a single-cycle mode adc conversion for for the specified input
 * on the ADC specified by the handle.
 *
 * @param adc_handle is handle relevent to the ADC performing the conversion.
 *
 * @param adc_input is the input on the ADC that's getting converted.
 *
 * @return error handling result code.
 */
aioc_error_t aioc_adc_convert_single_cycle(
  struct aioc_adc_dev* dev, 
  aioc_adc_input_t adc_input,
  uint16_t* result);


#endif  // AIOC_ADC_H
