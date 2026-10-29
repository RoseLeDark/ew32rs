#include "ew32_devm/ew32_config.h"
#include "ew32_devm/ew32_power.h"
#include <ew32_base/ew32_logging.h>
#include <ew32_devm/ew32_devm.h>
#include <stddef.h>


uint8_t _ew32_current_powerstate = 5;
uint8_t _ew32_soll_powserstate = 5;

ew32_semaphore_t __ew32_powerstate_lock;

uint8_t EW32_WEAK ew32_powsermanager_system_test(uint8_t curpwstate) {
    return curpwstate;
}

uint8_t EW32_WEAK ew32_powsermanager_prechange(uint8_t curstate, uint8_t sollstate) {
    return 0;
}
uint8_t EW32_WEAK ew32_powsermanager_afterchange(uint8_t isstate) {
    return 0;
}

void __ew32_powermanager_task(void* data) {
    uint8_t _soll = 5;
    uint8_t _current = 0;

    for( ;; ) {

        ew32_semaphore_lock(__ew32_powerstate_lock, EW32_INFINITY);
            _soll = _ew32_soll_powserstate;
            _current = _ew32_current_powerstate;
        ew32_semaphore_unlock(__ew32_powerstate_lock);

        if(_soll == _current) {
            _soll = ew32_powsermanager_system_test(_current);
        
            if(_soll != _current) {

                if(ew32_powsermanager_prechange(_current, _soll) == 0) {
                    

                    ew32_semaphore_lock(__ew32_powerstate_lock, EW32_INFINITY);
                    _ew32_current_powerstate = _soll;
                    ew32_devicemanager_powerstate_set(_ew32_current_powerstate);
                    ew32_semaphore_unlock(__ew32_powerstate_lock);

                    
                    ew32_powsermanager_afterchange(_soll);
                }
            }
        }
        ew32_task_delay(1000);
    }
}
ew32_task_t* __ew32_power_task;

uint8_t ew32_powermanager_probe(ew32_dev_t* device, void* cfg) {
    ew32_semaphore_create(EW32_SEMAPHORE_MUTEX, &__ew32_powerstate_lock);

    ew32_task_create(__ew32_powermanager_task, 2048, 2, "pmn0", NULL, 1, &__ew32_power_task);
    ew32_task_suspend(__ew32_power_task);
    return 0;
}
uint8_t ew32_powermanager_setup(ew32_dev_t* device, void* cfg) {
    ew32_task_resume(__ew32_power_task);
    return 0;
}
uint8_t ew32_powermanager_powerstate(ew32_dev_t* device, uint8_t state) {
    return 0;
}
int32_t ew32_powermanager_write(ew32_dev_t* device, void* data, int32_t size) {

    if(data != NULL && size == sizeof(ew32_powermanager_io_t)) {
        ew32_powermanager_io_t *_msg = (ew32_powermanager_io_t*)data;

        if(_msg->msg == EW32_POWERMANAGER_NEW_STATE) {

            ew32_semaphore_lock(__ew32_powerstate_lock, EW32_INFINITY);
            _ew32_soll_powserstate = _msg->state;
            ew32_semaphore_unlock(__ew32_powerstate_lock);
        }
    }

    return size;
}
int32_t ew32_powermanager_read(ew32_dev_t* device, void* data, int32_t size) {
    if(data != NULL && size == sizeof(ew32_powermanager_io_t)) {
        ew32_powermanager_io_t *_msg = (ew32_powermanager_io_t*)data;

        if(_msg->msg == EW32_POWERMANAGER_GET_STATE) {
            ew32_semaphore_lock(__ew32_powerstate_lock, EW32_INFINITY);
            _msg->state = _ew32_current_powerstate;
            ew32_semaphore_unlock(__ew32_powerstate_lock);
        }
    }
    return size;
}

ew32_dev_t __global_powsermanager;

uint8_t ew32_device_powermanager_setup(ew32_dev_t** device){
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;

    __global_powsermanager.subpath[0] = 'b';
    __global_powsermanager.subpath[1] = 'u';
    __global_powsermanager.subpath[2] = 's';
    __global_powsermanager.subpath[3] = EW32_VFS_DELIMETER;
    __global_powsermanager.subpath[4] = 'p';
    __global_powsermanager.subpath[5] = 'm';
    __global_powsermanager.subpath[6] = 'n';
    __global_powsermanager.subpath[7] = '0';

    // Instanz initialisieren
    __global_powsermanager.cls = ES32_DEVICE_CLASS_VIRTUAL;
    __global_powsermanager.type = EW32_DEVICE_TYPE_POWERMANAGER;
    __global_powsermanager.parent = NULL;
    __global_powsermanager.rw =  EW32_DEVICE_READ_WRITE;
    __global_powsermanager.powerstate = ew32_powermanager_powerstate;
    __global_powsermanager.probe = ew32_powermanager_probe;
    __global_powsermanager.setup = ew32_powermanager_setup;
    __global_powsermanager.write = ew32_powermanager_write;
    __global_powsermanager.read = ew32_powermanager_read;

    *device = &__global_powsermanager;


    return EW32_DEVICE_NO_ERROR;
}