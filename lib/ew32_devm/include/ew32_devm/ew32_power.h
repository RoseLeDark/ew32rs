/**
 * @file ew32_power.h
 * @brief Powermanager device
 */

#pragma once

#include "ew32_config.h"
#include "ew32_device.h"
#include "ew32_api.h"

#include <stdio.h>

#define EW32_POWERSTATE_L1                  1 // deep sleep
#define EW32_POWERSTATE_L2                  2
#define EW32_POWERSTATE_L3                  3
#define EW32_POWERSTATE_L4                  4
#define EW32_POWERSTATE_L5                  5  // Normal running

#define EW32_POWERMSG_TEST                  0 // call every n tiscks - return level you will change it
#define EW32_POWERMSG_PRE                   1 // can change? return 0 fpr ok and 1 for wait
#define EW32_POWERMSG_POST                  2 // after change

#ifdef __cplusplus
extern "C" {
#endif

uint8_t EW32_WEAK ew32_powsermanager_system_test(uint8_t curpwstate);
uint8_t EW32_WEAK ew32_powsermanager_prechange(uint8_t curstate, uint8_t sollstate);
uint8_t EW32_WEAK ew32_powsermanager_afterchange(uint8_t isstate);

#define EW32_POWERMANAGER_NEW_STATE         1
#define EW32_POWERMANAGER_GET_STATE         1

typedef struct ew32_powermanager_io {
    uint8_t msg;
    uint8_t state;
} ew32_powermanager_io_t;

uint8_t ew32_device_powermanager_setup(ew32_dev_t** device);

#ifdef __cplusplus
}
#endif