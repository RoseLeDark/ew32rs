
#include "ew32_config.h"

#if EW32_PLATFORM == EW32_PLATFORM_ESP
#include "ew32_i2c.h"

#include <stdio.h>
#include "esp_log.h"

#include "driver/i2c_master.h"


static i2c_master_bus_handle_t __esp32_i2c_bus_handle[EW32_DEVICE_I2C_MAX_DEVICES]; 
static i2c_master_dev_handle_t __esp32_i2c_dev_handle[128];


uint8_t ew32_i2c_probe(ew32_dev_t* device, void*) {
    ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;

    i2c_master_bus_handle_t bus_handle;

    if(handle->port_num < 2) {
        i2c_master_bus_config_t bus_config = {
            .i2c_port = handle->port_num,
            .sda_io_num = handle->pin_sda,
            .scl_io_num = handle->pin_scl,
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
            .flags.enable_internal_pullup = true,
        };
        
        // Temporäres Bus-Objekt für den Scan erzeugen
        if (i2c_new_master_bus(&bus_config, &bus_handle) != ESP_OK) {
            return EW32_DEVICE_ERROR_BUSY; 
        }

        ESP_LOGI("EW32_I2C", "Starting I2C scan ...");
        int __dev_num = 0;

        for (int addr = 0x00; addr <= 0x7F; addr++) {
            esp_err_t ret = i2c_master_probe(bus_handle, addr, 50);

            if (ret == ESP_OK) {
                ESP_LOGI("EW32_I2C", "Found addr: %02x", addr);
                handle->address_set[addr] = 1; 
                __dev_num++;
            } 
        }
        ESP_LOGI("EW32_I2C", "Scan complete: %d device(s) found.", __dev_num);

        // Wichtig: Temp-Handle direkt wieder abbauen, damit Setup ihn sauber initialisieren kann
        i2c_del_master_bus(bus_handle);
    }
    return EW32_DEVICE_NO_ERROR;
}
uint8_t ew32_i2c_setup(ew32_dev_t* device, void*) {
    ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;


    if(handle->port_num < 2) {
        i2c_master_bus_config_t bus_config = {
            .i2c_port = handle->port_num ,
            .sda_io_num = handle->pin_sda,
            .scl_io_num = handle->pin_scl,
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
            .flags.enable_internal_pullup = true,
        };
        i2c_new_master_bus(&bus_config, &__esp32_i2c_bus_handle[handle->port_num]);

        handle->apihandler = &__esp32_i2c_bus_handle[handle->port_num];

        
    }
#if EW32_DEVICE_USE_I2C_AUTOPROBE == 1
    for (uint8_t addr = 0x00; addr <= 0x7F; addr++) {
        
        if (handle->address_set[addr] == 1) {
            uint8_t _ret_hndl = 0;

            if(handle->handler != NULL) {
                _ret_hndl = handle->handler(device, EW32_BUSI2C_FOUND_NEWDEVICE, &addr, sizeof(uint8_t));
            }
            ESP_LOGI("EW32_I2C", "device at addrs: %02x @ %d", addr, _ret_hndl);
        }

    }
#endif

    return EW32_DEVICE_NO_ERROR;
}


int32_t ew32_i2c_read(ew32_dev_t* device, void* data, int32_t size) {
    if(data == NULL || device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(device->userdata == NULL) return EW32_DEVICE_ERROR_NULL;

    //ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;
    ew32_i2c_io_t *msg = (ew32_i2c_io_t*) data;
    esp_err_t _eret = 0;
    int8_t _ret = 0;

    if(msg->message_id == EW32_I2C_RECIVE_MSG_ID) {
        _eret = i2c_master_transmit_receive(__esp32_i2c_dev_handle[msg->addr], 
                                            msg->reg_addr, msg->reg_size,  
                                            msg->message, msg->msg_size, 
                                            msg->timeout);
        if(_eret != ESP_OK) _ret = -1;
        else _ret = msg->msg_size;
    }

    return _ret;
}

int32_t ew32_i2c_write(ew32_dev_t* device, void* data, int32_t size) {
    if(data == NULL || device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(device->userdata == NULL) return EW32_DEVICE_ERROR_NULL;


    ew32_i2c_cfg_t *handle = (ew32_i2c_cfg_t *)device->userdata;
    ew32_i2c_io_t *msg = (ew32_i2c_io_t*) data;
    esp_err_t _eret = 0;
    int8_t _ret = 0;

    if(msg->message_id == EW32_I2C_NEWDEV_MSG_ID) {
        ESP_LOGI("EW32_I2C", "Regist a new device ...");

        ew32_i2c_new_dev_t* _new_device = (ew32_i2c_new_dev_t*)msg->message;

        i2c_device_config_t dev_config = {
            .dev_addr_length = I2C_ADDR_BIT_LEN_7,
            .device_address = _new_device->new_addr,
            .scl_speed_hz = _new_device->scl_speed_hz,
            .scl_wait_us = _new_device->scl_wait_us
        };
        _eret = i2c_master_bus_add_device(handle->apihandler, &dev_config, &__esp32_i2c_dev_handle[_new_device->new_addr] );
        if(_eret != ESP_OK) _ret = -1;
        else _ret = 1;

    } else if (msg->message_id == EW32_I2C_TRANSMIT_MSG_ID ) {

        _eret = i2c_master_transmit(__esp32_i2c_dev_handle[msg->addr], msg->message, msg->msg_size, msg->timeout);
        if(_eret != ESP_OK) _ret = -1;
        else _ret = msg->msg_size;

    }

    return _ret;
}



uint8_t ew32_i2c_powerstate(ew32_dev_t* device, uint8_t state) {
    return EW32_DEVICE_ERROR_NOTIMPL;
}



#endif