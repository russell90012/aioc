#ifndef AIOC_HW_SIM
#define AIOC_HW_SIM

// MERCURY HEADER GOES HERE
// TBD

/**
 * @file:    aioc_hw_sim.h
 * @author:  Russell Shahenian
 * @since:   4/4/23
 * @brief:   This file contains the public AIOC HW simulation specifications.
 */

#include "aioc_defs.h"

#include <stddef.h>

//================================
// Public specifications.
//================================

void aioc_hw_sim_i2c_write(
  uint32_t dev_adrs,
  uint32_t reg_adrs,
  uint8_t* data,
  uint32_t data_count);

void  aioc_hw_sim_delay_ns(size_t delay);

void aioc_hw_sim_spi_transaction(uint32_t dev_id, uint32_t cs_id, uint8_t* data, uint32_t data_count);

#endif