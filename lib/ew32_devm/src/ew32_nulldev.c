#include "ew32_devm/ew32_config.h"
#include "ew32_devm/ew32_nulldev.h"
#include <ew32_base/ew32_logging.h>
#include <stddef.h>
#include <string.h>

uint8_t ew32_nulldev_probe(ew32_dev_t* device, void* cfg) {
    xewLoggingDebug("/dev/null", "ew32_nulldev_probe");

    return 0;
}
uint8_t ew32_nulldev_setup(ew32_dev_t* device, void* cfg) {
    xewLoggingDebug("/dev/null", "ew32_nulldev_setup");
    return 0;
}
uint8_t ew32_nulldev_powerstate(ew32_dev_t* device, uint8_t state) {
    xewLoggingDebug("/dev/null", "ew32_nulldev_powerstate");
    return 0;
}
int32_t ew32_nulldev_write(ew32_dev_t* device, void* data, int32_t size) {
    xewLoggingDebug("/dev/null", "ew32_nulldev_write");
    memset(data, 0, size);
    return 0;
}
int32_t ew32_nulldev_read(ew32_dev_t* device, void* data, int32_t size) {
    xewLoggingDebug("/dev/null", "ew32_nulldev_read");
    memset(data, 0, size);
    return 0;
}

ew32_dev_t __global_device;

uint8_t ew32_device_virtualnull_setup(ew32_dev_t* parent, ew32_dev_t** device) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;

    // Instanz initialisieren
    __global_device.subpath[0] = 'n';
    __global_device.subpath[1] = 'u';
    __global_device.subpath[2] = 'l';
    __global_device.subpath[3] = 'l';

    __global_device.cls = ES32_DEVICE_CLASS_VIRTUAL;
    __global_device.type = EW32_DEVICE_TYPE_STREAM;
    __global_device.parent = NULL;
    __global_device.rw =  EW32_DEVICE_READ_WRITE;
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