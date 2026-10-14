#include "ew32_device.h"
#include "string.h"


int32_t ew32_device_send(ew32_dev_t* device, void* data, int32_t size) {
    if(device == NULL) return -EW32_DEVICE_ERROR_NULL;
    if(device->write == NULL) return -EW32_DEVICE_ERROR_NOTIMPL;
    if(ew32_device_can_send(device) == EW32_CONFIG_NO) return -EW32_DEVICE_ERROR_NOT_SUPPORTED;

    int32_t datasended = 0;
    uint8_t i = 0;

    while(true) {
        if( i >= 3) break;

        datasended = device->write(device, data, size);
        if(datasended >= 0) break;

        i++;
    };

    return datasended;
}
int32_t ew32_device_read(ew32_dev_t* device, void* data, int32_t size) {
    if(device == NULL) return -EW32_DEVICE_ERROR_NULL;
    if(device->read == NULL) return -EW32_DEVICE_ERROR_NOTIMPL;
    if(ew32_device_can_read(device) == EW32_CONFIG_NO) return -EW32_DEVICE_ERROR_NOT_SUPPORTED;

    int32_t dataread = 0;
    uint8_t i = 0;

    while(true) {
        if( i >= 3) break;

        dataread = device->read(device, data, size);
        if(dataread >= 0) break;

        i++;
    };

    return dataread;
}
uint8_t ew32_device_pstate(ew32_dev_t* device, uint8_t state) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(device->powerstate == NULL) return EW32_DEVICE_ERROR_NOTIMPL; 

    uint8_t _ret = EW32_DEVICE_NO_ERROR;
    uint8_t i = 0;
    

    while(true) {
        if( i >= 3) break;
        
        _ret = device->powerstate(device, state);
        if(_ret == EW32_DEVICE_ERROR_WAIT) {
            i++;
            continue;
        } 
        break;
    }

    return _ret;
}
uint8_t ew32_device_probe(ew32_dev_t* device, void* cfg) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(device->probe == NULL) return EW32_DEVICE_ERROR_NOTIMPL; 

    uint8_t _ret = EW32_DEVICE_NO_ERROR;
    
    _ret = device->probe(device, cfg);

    return _ret;

}
uint8_t ew32_device_setup(ew32_dev_t* device, void* cfg) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(device->setup == NULL) return EW32_DEVICE_ERROR_NOTIMPL; 

    uint8_t _ret = EW32_DEVICE_NO_ERROR;
    
    _ret = device->setup(device, cfg);

    return _ret;
}
uint8_t ew32_device_set_userdata(ew32_dev_t* device, void* data) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;

    device->userdata = data;

    return EW32_DEVICE_NO_ERROR;
}
uint8_t ew32_device_get_userdata(ew32_dev_t* device, void** data) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    uint8_t _ret = EW32_DEVICE_NO_ERROR;

    if(data != NULL) *data = device->userdata;

    return _ret;
}

uint8_t ew32_device_can_send(ew32_dev_t* device) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    return (device->type == EW32_DEVICE_TYPE_OUTPUT || device->type == EW32_DEVICE_TYPE_INOUT) ? 
        EW32_CONFIG_YES : EW32_CONFIG_NO;
}


uint8_t ew32_device_can_read(ew32_dev_t* device) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    return (device->type == EW32_DEVICE_TYPE_INPUT || device->type == EW32_DEVICE_TYPE_INOUT) ? 
        EW32_CONFIG_YES : EW32_CONFIG_NO;
}


uint8_t ew32_device_get_parent(ew32_dev_t* device, ew32_dev_t** parent) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;
    if(parent != NULL) *parent = device->parent;

    return  (device->parent != NULL ? EW32_CONFIG_YES : EW32_CONFIG_NO);
}
uint8_t ew32_device_set_parent(ew32_dev_t* device, ew32_dev_t* parent) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;

    device->parent = parent;


    return EW32_DEVICE_NO_ERROR;
}