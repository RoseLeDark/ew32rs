#include "ew32_config.h"
#include "ew32_nulldev.h"

uint8_t ew32_nulldev_probe(ew32_dev_t* device, void* cfg) {
    return 0;
}
uint8_t ew32_nulldev_setup(ew32_dev_t* device, void* cfg) {
    return 0;
}
uint8_t ew32_nulldev_powerstate(ew32_dev_t* device, uint8_t state) {
    return 0;
}
int32_t ew32_nulldev_write(ew32_dev_t* device, void* data, int32_t size) {
    return size;
}
int32_t ew32_nulldev_read(ew32_dev_t* device, void* data, int32_t size) {
    return size;
}

ew32_dev_t __global_device;

uint8_t ew32_device_virtualnull_setup(ew32_dev_t* parent, ew32_dev_t** device) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;

    // Instanz initialisieren
    __global_device.cls = ES32_DEVICE_CLASS_VIRTUAL;
    __global_device.type = EW32_DEVICE_TYPE_NONE;
    __global_device.parent = NULL;
    __global_device.powerstate = ew32_nulldev_powerstate;
    __global_device.probe = ew32_nulldev_probe;
    __global_device.setup = ew32_nulldev_setup;
    __global_device.write = ew32_nulldev_write;
    __global_device.read = ew32_nulldev_read;

    __global_device.userdata = NULL;
    __global_device.parent = parent;

    *device = &__global_device;

    return EW32_DEVICE_NO_ERROR;
}