#include "ew32_config.h"
#include "ew32_i2c.h"

#if EW32_DEVICE_USE_I2C == 1
#include "driver/i2c_master.h"

extern uint8_t ew32_i2c_probe(ew32_dev_t* device, void* cfg);
extern uint8_t ew32_i2c_setup(ew32_dev_t* device, void* cfg);
extern uint8_t ew32_i2c_powerstate(ew32_dev_t* device, uint8_t state);
extern int32_t ew32_i2c_write(ew32_dev_t* device, void* data, int32_t size);
extern int32_t ew32_i2c_read(ew32_dev_t* device, void* data, int32_t size);


static ew32_dev_t __i2c_devices[EW32_DEVICE_I2C_MAX_DEVICES];     
static ew32_i2c_cfg_t __global_cfg[EW32_DEVICE_I2C_MAX_DEVICES];
static uint8_t __i2c_device_used[EW32_DEVICE_I2C_MAX_DEVICES];


uint8_t ew32_device_i2c_setup(uint8_t sda, uint8_t scl, uint8_t port_num, i2c_handler handler, ew32_dev_t** device) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(port_num >= EW32_DEVICE_I2C_MAX_DEVICES) return 10;
    if(__i2c_device_used[port_num] == 1) return 11;

    __i2c_device_used[port_num] = 1;

    __global_cfg[port_num].pin_scl = scl;
    __global_cfg[port_num].pin_sda = sda;
    __global_cfg[port_num].port_num = port_num;
    __global_cfg[port_num].handler = handler;
    __global_cfg[port_num].hardware = port_num > I2C_NUM_MAX; // port 0 und 1 used hardware i2c

   // Instanz initialisieren
    __i2c_devices[port_num].cls = ES32_DEVICE_CLASS_SYSTEM;
    __i2c_devices[port_num].type = EW32_DEVICE_TYPE_INOUT;
    __i2c_devices[port_num].parent = NULL;
    __i2c_devices[port_num].powerstate = ew32_i2c_powerstate;
    __i2c_devices[port_num].probe = ew32_i2c_probe;
    __i2c_devices[port_num].setup = ew32_i2c_setup;
    __i2c_devices[port_num].write = ew32_i2c_write;
    __i2c_devices[port_num].read = ew32_i2c_read;

    __i2c_devices[port_num].userdata = &__global_cfg[port_num];

    *device = &__i2c_devices[port_num];
    

    return EW32_DEVICE_NO_ERROR;
}

#endif
