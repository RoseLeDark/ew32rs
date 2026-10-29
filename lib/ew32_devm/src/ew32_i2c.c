#include "ew32_devm/ew32_config.h"
#include "ew32_devm/ew32_i2c.h"

#include "string.h"

#if EW32_DEVICE_USE_I2C == 1

extern uint8_t ew32_i2c_probe(ew32_dev_t* device, void* cfg);
extern uint8_t ew32_i2c_setup(ew32_dev_t* device, void* cfg);
extern uint8_t ew32_i2c_powerstate(ew32_dev_t* device, uint8_t state);
extern int32_t ew32_i2c_write(ew32_dev_t* device, void* data, int32_t size);
extern int32_t ew32_i2c_read(ew32_dev_t* device, void* data, int32_t size);
extern uint8_t ew32_i2c_isadress(ew32_dev_t* device, uint8_t address);

static ew32_dev_t __i2c_devices[EW32_DEVICE_I2C_MAX_DEVICES];     
static ew32_i2c_cfg_t __global_cfg[EW32_DEVICE_I2C_MAX_DEVICES];
static uint8_t __i2c_device_used[EW32_DEVICE_I2C_MAX_DEVICES];


uint8_t ew32_device_i2c_setup(uint8_t sda, uint8_t scl, uint8_t port_num, i2c_handler handler, ew32_dev_t** device) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(port_num >= EW32_DEVICE_I2C_MAX_DEVICES) return EW32_DEVICE_ERROR_INVALID_ARG;
    if(__i2c_device_used[port_num] == 1) return EW32_DEVICE_ERROR_ALLREADY_INIT;

    __i2c_device_used[port_num] = 1;

    
    
    __global_cfg[port_num].pin_scl = scl;
    __global_cfg[port_num].pin_sda = sda;
    __global_cfg[port_num].port_num = port_num;
    __global_cfg[port_num].handler = handler;
    __global_cfg[port_num].hardware = port_num > 2; // port 0 und 1 used hardware i2c

    
    __i2c_devices[port_num].subpath[0] = 'b';
    __i2c_devices[port_num].subpath[1] = 'u';
    __i2c_devices[port_num].subpath[2] = 's';
    __i2c_devices[port_num].subpath[3] = EW32_VFS_DELIMETER;
    __i2c_devices[port_num].subpath[4] = 'i';
    __i2c_devices[port_num].subpath[5] = '2';
    __i2c_devices[port_num].subpath[6] = 'c';
    __i2c_devices[port_num].subpath[7] = port_num +'0';


   // Instanz initialisieren
    __i2c_devices[port_num].cls = ES32_DEVICE_CLASS_BUS;
    __i2c_devices[port_num].type = EW32_DEVICE_TYPE_I2C;
    __i2c_devices[port_num].parent = NULL;
    __i2c_devices[port_num].rw =  EW32_DEVICE_READ_WRITE;
    __i2c_devices[port_num].powerstate = ew32_i2c_powerstate;
    __i2c_devices[port_num].probe = ew32_i2c_probe;
    __i2c_devices[port_num].setup = ew32_i2c_setup;
    __i2c_devices[port_num].write = ew32_i2c_write;
    __i2c_devices[port_num].read = ew32_i2c_read;

    __i2c_devices[port_num].userdata = &__global_cfg[port_num];

    *device = &__i2c_devices[port_num];
    

    return EW32_DEVICE_NO_ERROR;
}
uint8_t ew32_device_i2c_isadress(uint8_t port_num, uint8_t address)  {
    if(port_num >= EW32_DEVICE_I2C_MAX_DEVICES) return EW32_DEVICE_ERROR_INVALID_ARG;
    if(__i2c_device_used[port_num] != 1) return EW32_DEVICE_ERROR_NOT_USED; // not in use


    return ew32_i2c_isadress(&__i2c_devices[port_num], address);
}


int32_t ew32_device_i2c_read(uint8_t port_num,  ew32_i2c_io_t* data, int32_t size) {
    if(port_num >= EW32_DEVICE_I2C_MAX_DEVICES) return EW32_DEVICE_ERROR_INVALID_ARG;
    if(__i2c_device_used[port_num] != 1) return EW32_DEVICE_ERROR_NOT_USED; // not in use

    return ew32_i2c_read(&__i2c_devices[port_num], data, size);
}

int32_t ew32_device_i2c_write(uint8_t port_num,  ew32_i2c_io_t* data, int32_t size) {
    if(port_num >= EW32_DEVICE_I2C_MAX_DEVICES) return EW32_DEVICE_ERROR_INVALID_ARG;
    if(__i2c_device_used[port_num] != 1) return EW32_DEVICE_ERROR_NOT_USED; // not in use

    return ew32_i2c_write(&__i2c_devices[port_num], data, size);
}

#endif
