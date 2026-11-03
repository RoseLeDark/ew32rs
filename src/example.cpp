#include <Arduino.h>
#include <Wire.h>

#include "ew32_devm/ew32_api.h"
#include "ew32_base/ew32_logging.h"
#include "ew32_devm/ew32_devm.h"
#include "ew32_devm/ew32_vfs.h"
#include "ew32_devm/ew32_power.h"

#include <stddef.h>
#include <string.h>

/** 
 * Example Output on ESP32 - Arduino
 * --- More details at https://bit.ly/pio-monitor-filters
 * --- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
 * [1:MAIN] Initializing VFS...[0:/dev/null] ew32_nulldev_probe
 * [0:/dev/null] ew32_nulldev_setup
 * [0:DEVM] regist new device with id: 6
 * [0:/dev/test0] ew32_testdev_probe
 * [0:/dev/test0] ew32_testdev_setup
 * [0:/dev/test0] set power state to: 5
 * [1:MAIN] Dumping device manager state
 * [0:EW32_DEVM] Device 0: /dev/bus/i2c0, Probe State: 4, Power State: 5
 * [0:EW32_DEVM] Device 1: /dev/bus/pmn0, Probe State: 4, Power State: 5
 * [0:EW32_DEVM] Device 2: /dev/null, Probe State: 4, Power State: 5
 * [0:EW32_DEVM] Device 3: /dev/stdout, Probe State: 4, Power State: 5
 * [0:EW32_DEVM] Device 4: /dev/stderr, Probe State: 4, Power State: 5
 * [0:EW32_DEVM] Device 5: /dev/stdin, Probe State: 4, Power State: 5
 * [0:EW32_DEVM] Device 6: /dev/test0, Probe State: 4, Power State: 5
 * [0:PM] set to power state: 2
 * [0:/dev/null] ew32_nulldev_set_state
 * [1:MAIN] Hallo Welt!
 * [1:MAIN] Hallo Welt!
 * [1:MAIN] Hallo Welt!
 * [1:MAIN] Hallo Welt!
 * [1:MAIN] Hallo Welt!
 * [1:MAIN] Hallo Welt!
 * [1:MAIN] Hallo Welt!
 * ....
 */
uint8_t ew32_testdev_probe(ew32_dev_t* device, void* cfg) {
    xewLoggingDebug("/dev/test0", "ew32_testdev_probe\n");

    return 0;
}
uint8_t ew32_testdev_setup(ew32_dev_t* device, void* cfg) {
    xewLoggingDebug("/dev/test0", "ew32_testdev_setup\n");
    return 0;
}
uint8_t ew32_testdev_set_state(ew32_dev_t* device, uint8_t stateid, uint8_t state) {

    if(stateid == EW32_DEVICE_STATEID_POWERSTATE)
        xewLoggingDebug1("/dev/test0", "set power state to: %d\n", state);

    return 0;
}

uint8_t ew32_testdev_get_state(ew32_dev_t* device, uint8_t state) {
    return EW32_POWERSTATE_L5;
}

int32_t ew32_testdev_write(ew32_dev_t* device, void* data, int32_t size) {
    xewLoggingDebug("/dev/test0", "ew32_testdev_write\n");
    memset(data, 0, size);
    return 0;
}
int32_t ew32_testdev_read(ew32_dev_t* device, void* data, int32_t size) {
    xewLoggingDebug("/dev/test0", "ew32_testdev_read\n");
    memset(data, 0, size);
    return 0;
}

ew32_dev_t __global_device_test;

uint8_t ew32_device_test0_setup(ew32_dev_t** device) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;

    // Instanz initialisieren
    __global_device_test.subpath[0] = 't';
    __global_device_test.subpath[1] = 'e';
    __global_device_test.subpath[2] = 's';
    __global_device_test.subpath[3] = 't';
    __global_device_test.subpath[4] = '0'; 


    __global_device_test.cls = ES32_DEVICE_CLASS_VIRTUAL;
    __global_device_test.type = EW32_DEVICE_TYPE_STREAM;
    __global_device_test.parent = NULL;
    __global_device_test.rw =  EW32_DEVICE_READ_WRITE;
    __global_device_test.get_state = ew32_testdev_get_state;
    __global_device_test.set_state = ew32_testdev_set_state;
    __global_device_test.probe = ew32_testdev_probe;
    __global_device_test.setup = ew32_testdev_setup;
    __global_device_test.write = ew32_testdev_write;
    __global_device_test.read = ew32_testdev_read;

    __global_device_test.userdata = NULL;
    __global_device_test.parent = 0;
    __global_device_test.on_run_r = NULL;
    

    *device = &__global_device_test;
    return EW32_DEVICE_NO_ERROR;
}



int8_t __logging_handler(uint8_t level, const char* tag, const char* text) {
   Serial.printf("[%d:%s] %s", level, tag, text); return 0;
}

// example the baterry is by 20% then switch to EW32_POWERSTATE_L1. EW32_POWERSTATE_l5 is full powerstate
uint8_t _ew32_power_change_handle(uint8_t curpwstate) {
    // gib zurück in welchen powserstate gewechselt werden soll EW32_POWERSTATE_L1 - EW32_POWERSTATE_L5 gloabal all devices
    return EW32_POWERSTATE_L5;
}

// return 0 when the cheak ok is, and 1 on wait
uint8_t __ew32_power_change_cheak(uint8_t curstate, uint8_t sollstate) {
    return 0;
}



void setup() {
   Serial.begin(115200);
   xewLoggingInitEx(&__logging_handler, EW32_LOGGING_LEVEL_DEBUG, EW32_LOGGING_LEVEL_CRITICAL);

    ew32_tasksystem_init();
    
    ew32_dev_t* __test_device = (ew32_dev_t*)malloc(sizeof(ew32_dev_t));

    xewLoggingSerial(EW32_LOGGING_LEVEL_INFO, "MAIN", "Initializing VFS...");
    if(ew32_vfs_init() != 0) {
        xewLoggingDebug("MAIN", "Initializing VFS ... [Error]\n");
    }

    if(ew32_devicemanager_create() != 0) {
        xewLoggingDebug("MAIN", "Device Manager Create ... [Error]\n");
    } 
   ew32_device_test0_setup(&__test_device);
   ew32_devicemanager_registdevice(__test_device, 0, 0);

   ew32_device_powermanager_sethandle(_ew32_power_change_handle, __ew32_power_change_cheak);


    ew32_devicemanager_probeall();
    ew32_devicemanager_setup();
    if(ew32_devicemanager_run() != 0) {
      xewLoggingDebug("MAIN", "Device Manager run ... [Error]\n");
    }

    xewLoggingSerial(EW32_LOGGING_LEVEL_INFO, "MAIN", "Dumping device manager state\n");
    ew32_devicemanager_dump(EW32_DEVICEMANAGER_STATE_RUNNING);
    
    
   ewi32_devicemanager_setstate(EW32_DEVICEMANAGER_ALL, EW32_DEVICE_STATEID_MODE, EW32_DEVICE_STATE_RESUME);
}


static unsigned long __previousMillis = ew32_task_get_ticks(); 

void loop(void) {
     unsigned long curticks = ew32_task_get_ticks();

   ew32_devicemanager_loop(NULL);

    if ((curticks - __previousMillis) >= 2000) {
        __previousMillis = ew32_task_get_ticks();
        xewLoggingSerial(EW32_LOGGING_LEVEL_INFO, "MAIN", "Hallo Welt!\n");
    }
}
