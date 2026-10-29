#include "ew32_devm/ew32_devm.h"

#include <stdio.h>
#include <string.h>

#include "ew32_devm/ew32_i2c.h"
#include "ew32_devm/ew32_vfs.h"
#include "ew32_devm/ew32_power.h"
#include "ew32_devm/ew32_nulldev.h"
#include "ew32_devm/ew32_std.h"

typedef struct ew32_devmanager_entry {
    ew32_dev_t* device;
    uint8_t     powerstate;  // 0 = off, 5 = enable
    int8_t      probe_state; // = 0 (no probe), 1 (probe), 2 (init/run) - (-1= not used), -10 = error
    void*       cfg;
} ew32_devm_entry_t;  

ew32_devm_entry_t __devman[EW32_DEVICE_MANAGER_DEVICES];
uint8_t           __devm_global_state = 0;


void __ew32_devicemanager_add_device(uint8_t id) {
    ew32_device_probe(__devman[id].device, NULL);
    ew32_device_setup(__devman[id].device, NULL);
    __devman[id].powerstate = 5;
    __devman[id].probe_state = EW32_DEVICEMANAGER_STATE_RUNNING;
    __devman[id].device->id = ew32_vfsdb_regist_device(__devman[id].device->subpath, __devman[id].device, 1);
}
uint8_t ew32_devicemanager_create(){
    if(__devm_global_state != 0) return 0;
    __devm_global_state = EW32_DEVICEMANAGER_STATE_INIT;
    
    for(int i=1; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        __devman[i].device = NULL;
        __devman[i].powerstate = 0;
        __devman[i].probe_state = -1;
    }
    
    

    /// setup i2c
    ew32_device_i2c_setup(21, 22, 0, NULL, &__devman[0].device);
    __ew32_devicemanager_add_device(0);

    /// setup powermanager
    ew32_device_powermanager_setup(&__devman[1].device);
    __ew32_devicemanager_add_device(1);

    // NULL device
    ew32_device_virtualnull_setup(NULL, &__devman[2].device);
    __ew32_devicemanager_add_device(2);

    ew32_dev_std_setup(EW32_STDOUT, &__devman[3].device);
    __ew32_devicemanager_add_device(3);

    ew32_dev_std_setup(EW32_STDERR, &__devman[4].device);
    __ew32_devicemanager_add_device(4);

    ew32_dev_std_setup(EW32_STDIN, &__devman[5].device);
    __ew32_devicemanager_add_device(5);

    return 0;
}
uint8_t ew32_devicemanager_registdevice(ew32_dev_t* device, void* cfg, uint8_t preInit) {
    int16_t _id = -1;
    uint8_t _ret = 0;

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].probe_state == -1) {
            __devman[i].probe_state = 0;
            _id = i;
            break;;
        }
    }

    if(_id == -1) return EW32_DEVICEMANAGER_FULL;

    __devman[_id].device = device;
    __devman[_id].cfg = cfg;


    if(__devm_global_state >= EW32_DEVICEMANAGER_STATE_PROBE || preInit == 1) {// allready_probe
        _ret = ew32_device_probe(__devman[_id].device, __devman[_id].cfg);

        if(_ret != EW32_DEVICE_NO_ERROR)   {
            __devman[_id].probe_state = EW32_DEVICEMANAGER_STATE_PROBE;

            if(__devm_global_state >= EW32_DEVICEMANAGER_STATE_SETUP || preInit == 1) {
                _ret = ew32_device_setup(__devman[_id].device, __devman[_id].cfg);
                
                if(_ret != EW32_DEVICE_NO_ERROR)   { 
                    __devman[_id].probe_state = EW32_DEVICEMANAGER_STATE_SETUP;
                } 
            }
        }
    }
    if(preInit == 1) {
        __devman[_id].device->id = ew32_vfsdb_regist_device(__devman[_id].device->subpath, 
                                                      __devman[_id].device, 
                                                      _ret == EW32_DEVICE_NO_ERROR ? 1 : 0);
    }
    

    return _ret;
}

uint8_t ew32_devicemanager_probeall() {
    if(__devm_global_state != EW32_DEVICEMANAGER_STATE_INIT) return 0;
    __devm_global_state = EW32_DEVICEMANAGER_STATE_PROBE; 

    uint8_t _devret = 0;
    uint16_t _error = 0;

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].probe_state == 0) {
            _devret = ew32_device_probe(__devman[i].device, __devman[i].cfg);
            if (_devret == 0) __devman[i].probe_state = EW32_DEVICEMANAGER_STATE_PROBE ;
            else { 
                __devman[i].probe_state = _devret * -10; 
                _error++; 
            }
        }
    }

    

    return (_error == 0) ? 0 : 1;
}

uint8_t ew32_devicemanager_setup() {
    if(__devm_global_state != EW32_DEVICEMANAGER_STATE_PROBE) return 0;
    __devm_global_state = EW32_DEVICEMANAGER_STATE_SETUP;

    uint8_t _devret = 0;
    uint16_t _error = 0;

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_PROBE) {
            _devret = ew32_device_setup(__devman[i].device, __devman[i].cfg);
            if (_devret == 0)  __devman[i].probe_state = EW32_DEVICEMANAGER_STATE_SETUP ;
            else { 
                __devman[i].probe_state = _devret * -10; 
                _error++; 
            }

            __devman[i].device->id = ew32_vfsdb_regist_device(__devman[i].device->subpath, 
                                                      __devman[i].device, 
                                                      _devret == EW32_DEVICE_NO_ERROR ? 1 : 0);
        }
    }
    return (_error == 0) ? 0 : 1;
}
uint8_t ew32_devicemanager_run() {
    if(__devm_global_state != EW32_DEVICEMANAGER_STATE_SETUP) return 0;
    __devm_global_state = EW32_DEVICEMANAGER_STATE_RUNNING;


    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_SETUP) {
            ew32_device_pstate(__devman[i].device, 5);
            __devman[i].probe_state = EW32_DEVICEMANAGER_STATE_RUNNING ;


        }
    }
    return 0;
}

void ew32_devicemanager_powerstate_set(uint8_t newState) {

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++) {
        if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_RUNNING) {
            ew32_device_pstate(__devman[i].device, newState);
        }
    }
}

uint16_t ew32_devicemanager_get_running() {
    if(__devm_global_state != EW32_DEVICEMANAGER_STATE_RUNNING) return 0;

    uint16_t _ret = 0;

    for(int i=0; i< EW32_DEVICE_MANAGER_DEVICES; i++)  {
        if(__devman[i].probe_state == EW32_DEVICEMANAGER_STATE_RUNNING)
            _ret++;
    }
    return _ret;
}