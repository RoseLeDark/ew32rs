/**
 * @file ew32_std.h
 * @brief stdout/stdint/stderr
 */

#pragma once

#include "ew32_config.h"
#include "ew32_device.h"
#include "ew32_api.h"

#include <stdio.h>

#define EW32_STDOUT  0
#define EW32_STDERR  1
#define EW32_STDIN   2

#ifdef __cplusplus
extern "C" {
#endif

uint8_t ew32_dev_std_setup(const uint8_t id, ew32_dev_t** device);

void ew32_dev_std_setdevice(const uint8_t id, ew32_dev_t* parent);

#ifdef __cplusplus
}
#endif
