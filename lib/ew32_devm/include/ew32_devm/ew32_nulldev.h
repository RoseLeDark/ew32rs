/**
 * @file ew32_nulldev.h
 * @brief Virtual null/fallback device driver header for the ew32rs framework.
 */

#pragma once

#include "ew32_device.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes and assigns a virtual null fallback device node.
 * 
 * Sets up a placeholder device structure with dummy handlers to prevent 
 * null-pointer dereferences for discovered hardware nodes that lack a specific driver.
 * 
 * @param parent Pointer to the parent device (e.g., the bus controller).
 * @param device Double pointer to receive the created virtual device instance.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success, or an error code on failure.
 */
uint8_t ew32_device_virtualnull_setup(ew32_dev_t* parent, ew32_dev_t** device);

#ifdef __cplusplus
}
#endif