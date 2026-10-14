#include "ew32_device.h"
#include "ew32_i2c.h"
#include "ew32_nulldev.h"

ew32_dev_t* __dev_i2c;

uint8_t __i2c_handler( ew32_dev_t* sender, uint8_t msg, void* addrs, uint32_t size) {
    if(msg == EW32_BUSI2C_FOUND_NEWDEVICE) {

        ew32_dev_t* new_device;

        uint8_t __new = *((uint8_t*)addrs);

        ew32_i2c_new_dev_t dev = {
            .new_addr = __new,
            .scl_speed_hz = 20,
            .scl_wait_us = 100
        };
        ew32_i2c_io_t msg = {
            .message = &dev,
            .msg_size = sizeof(dev),
            .timeout = 1000,
            .addr =255,
            .message_id = EW32_I2C_NEWDEV_MSG_ID
        };

        int8_t _ret = ew32_device_send(sender, &msg, sizeof(msg)); // regist a new device demo !
        if(_ret > 0) {
            ew32_device_virtualnull_setup(sender, &new_device);
            return EW32_DEVICE_NO_ERROR;
        }

        
    }
    return EW32_DEVICE_ERROR_NOT_SUPPORTED;
}


void setup() {
    ew32_device_i2c_setup(5, 4, 0, __i2c_handler, &__dev_i2c);
    ew32_device_probe(__dev_i2c, NULL);
    ew32_device_setup(__dev_i2c, NULL);
}


void move() {

}

void run() { 


}

void app_main() {
    setup();

    while(true) {
        move();
        run();
    }
}