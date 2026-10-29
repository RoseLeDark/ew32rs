#pragma once

#include "ew32_device.h"

#define EW32_DEVICEMANAGER_STATE_INIT       1
#define EW32_DEVICEMANAGER_STATE_PROBE      2
#define EW32_DEVICEMANAGER_STATE_SETUP      3
#define EW32_DEVICEMANAGER_STATE_RUNNING    4

#ifdef __cplusplus
extern "C" {
#endif

uint8_t ew32_devicemanager_create();
uint8_t ew32_devicemanager_registdevice(ew32_dev_t* device,  void* cfg, uint8_t preInit);
uint8_t ew32_devicemanager_get_device(const char* path, ew32_dev_t** device);

uint8_t ew32_devicemanager_probeall();
uint8_t ew32_devicemanager_setup();
uint8_t ew32_devicemanager_run();

uint16_t ew32_devicemanager_get_running();

void ew32_devicemanager_powerstate_set(uint8_t newState);
#ifdef __cplusplus
}
#endif