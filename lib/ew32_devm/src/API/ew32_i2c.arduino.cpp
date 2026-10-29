
#include "ew32_devm/ew32_config.h"

#if EW32_PLATFORM == EW32_PLATFORM_ARDUINO ||  EW32_PLATFORM_RPPICO 
#include "ew32_devm/ew32_i2c.h"
#include "ew32_base/ew32_logging.h"

#include <stdio.h>

#include <Wire.h>

extern "C" {

uint8_t ew32_i2c_isadress(ew32_dev_t* device, uint8_t address) {
     ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;

    return (handle->address_set[address]); 
}

uint8_t ew32_i2c_probe(ew32_dev_t* device, void*) {
    ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;

    uint8_t error;
    
    TwoWire &wire = handle->port_num == 0 ? Wire : Wire1;

    #if EW32_PLATFORM == EW32_PLATFORM_RPPICO  
        wire.setSDA(handle->pin_sda);
        wire.setSCL(handle->pin_scl);
        wire.begin();
    #else
        wire.begin(handle->pin_sda,handle->pin_scl);
    #endif

    for (uint8_t address  = 1; address < 127; address++) {
        wire.beginTransmission(address);
        error = wire.endTransmission();

        if (error == 0) {
            handle->address_set[address] = 1;
            xewLoggingInfo1("I2C", "Found device 0x%02X", address);
        }
        
    }

    return EW32_DEVICE_NO_ERROR;
}
uint8_t ew32_i2c_setup(ew32_dev_t* device, void*) {
    ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;
    TwoWire &wire = handle->port_num == 0 ? Wire : Wire1;

    #if EW32_PLATFORM == EW32_PLATFORM_RPPICO 
        wire.setSDA(handle->pin_sda);
        wire.setSCL(handle->pin_scl);
        wire.begin();
    #else
        wire.begin(handle->pin_sda,handle->pin_scl);
    #endif
    
#if EW32_DEVICE_USE_I2C_AUTOPROBE == 1
    for (uint8_t addr = 0x00; addr <= 0x7F; addr++) {
        
        if (handle->address_set[addr] == 1) {
            uint8_t _ret_hndl = 0;

            if(handle->handler != NULL) {
                _ret_hndl = handle->handler(device, EW32_BUSI2C_FOUND_NEWDEVICE, &addr, sizeof(uint8_t));
            }
            xewLoggingSerial(EW32_LOGGING_LEVEL_INFO, "I2C", "device at addrs: %02x @ %d", addr, _ret_hndl);
        }

    }
#endif

    return EW32_DEVICE_NO_ERROR;
}


int32_t ew32_i2c_read(ew32_dev_t* device, void* data, int32_t size) {
    if(data == NULL || device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(device->userdata == NULL) return EW32_DEVICE_ERROR_NULL;

    

    ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;
    ew32_i2c_io_t *msg = (ew32_i2c_io_t*) data;

    int8_t _ret = 0;

    TwoWire &wire = handle->port_num == 0 ? Wire : Wire1;

    if(msg->message_id == EW32_I2C_RECIVE_MSG_ID) {

        wire.beginTransmission(msg->addr);
        wire.write((uint8_t*)msg->message, msg->msg_size);
         /// Errors:
    //  0 : Success
    //  1 : Data too long
    //  2 : NACK on transmit of address
    //  3 : NACK on transmit of data
    //  4 : Other error
    //  5 : Timeouts
        _ret = wire.endTransmission();
        if(_ret != 0) {
            _ret = -_ret; 
        } else {
            wire.requestFrom(msg->addr, msg->reg_size);
            int i = 0;
            while (wire.available()){
                msg->reg_addr[i] = wire.read(); 
                i = i + 1;
            }
            _ret = i;
        }
    }

    return _ret;
}

int32_t ew32_i2c_write(ew32_dev_t* device, void* data, int32_t size) {
    if(data == NULL || device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(device->userdata == NULL) return EW32_DEVICE_ERROR_NULL;


    ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;
    ew32_i2c_io_t *msg = (ew32_i2c_io_t*) data;

    int8_t _ret = 0;

    TwoWire &wire = handle->port_num == 0 ? Wire : Wire1;

    if(msg->message_id == EW32_I2C_NEWDEV_MSG_ID) {
        _ret = 0;
    } else if (msg->message_id == EW32_I2C_TRANSMIT_MSG_ID ) {

        wire.beginTransmission(msg->addr);
        wire.write(msg->message, msg->msg_size);
        _ret =  wire.endTransmission();

        if(_ret != 0) _ret = -_ret;

    }
    return _ret;
}



uint8_t ew32_i2c_powerstate(ew32_dev_t* device, uint8_t state) {
    return EW32_DEVICE_ERROR_NOTIMPL;
}


}
#endif