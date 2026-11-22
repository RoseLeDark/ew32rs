
#include "ew32_devm/devmanager.h"

int8_t __logging_handler(uint8_t level, const char* tag, const char* text) {

#if EW32_DEVICE_USE_UART == EW32_CONFIG_YES
    char buf[256];

    sprintf(buf, "[%d:%s] %s", level, tag, text);
    ew32_device_uart_send(EW32_DEVICE_UART_STDPORT, buf, strlen(buf));
#endif

    return 0;
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
    ew32_osa_task_init();

   xewLoggingInitEx(&__logging_handler, EW32_LOGGING_LEVEL_DEBUG, EW32_LOGGING_LEVEL_CRITICAL);

        

    if(ew32_vfs_init() != 0) {
        xewLoggingDebug("MAIN", "Initializing VFS ... [Error]\n");
    } else {
         xewLoggingDebug("MAIN", "Initializing VFS ... [OK]\n");
    }

    if(ew32_devicemanager_create() != 0) {
        xewLoggingDebug("MAIN", "Device Manager Create ... [Error]\n");
    }  else {
        xewLoggingDebug("MAIN", "Device Manager Create ... [OK]\n");
    }
    

    ew32_device_powermanager_sethandle(_ew32_power_change_handle, __ew32_power_change_cheak);

    ew32_devicemanager_probeall();
    ew32_devicemanager_setup();
    if(ew32_devicemanager_run() != 0) {
        xewLoggingDebug("MAIN", "Device Manager run ... [Error]\n");
    } else {
        xewLoggingDebug("MAIN", "Device Manager run ... [OK]\n");
    }

    ew32_devicemanager_dump(EW32_DEVICEMANAGER_STATE_ALL);
    
    

    ew32_devicemanager_setstate_by_id(EW32_DEVICEMANAGER_ALL, EW32_DEVICE_STATEID_MODE, EW32_DEVICE_STATE_RESUME);

}

void loop(void) {
    ew32_devicemanager_loop(NULL);
}
