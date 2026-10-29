#include "ew32_devm/ew32_std.h"

#include "ew32_devm/ew32_config.h"
#include "ew32_devm/ew32_std.h"
#include <ew32_base/ew32_logging.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define EW32_DEVICE_CLASS_INSTD     0x99

uint8_t ew32_std_probe(ew32_dev_t* device, void* cfg) {
    return 0;
}
uint8_t ew32_std_setup(ew32_dev_t* device, void* cfg) {
    return 0;
}
uint8_t ew32_std_powerstate(ew32_dev_t* device, uint8_t state) {
    return 0;
}
int32_t ew32_std_write(ew32_dev_t* device, void* data, int32_t size) {
    if(device == NULL) return -1;

    char* __inf = (char*)data;
    int _user = device->raw_userdata;
    ew32_dev_t *parent = device->parent;
    int32_t _ret = size;

    if(_user == EW32_STDOUT) {
        if(parent == NULL) xewLoggingInfo("stdout", __inf); 
        else _ret = ew32_device_send(parent, data, size);
    } else if(_user == EW32_STDERR) {
        if(parent == NULL) xewLoggingError("stdout", __inf); 
        else _ret = ew32_device_send(parent, data, size);
    }

    return size;
}
int32_t ew32_std_read(ew32_dev_t* device, void* data, int32_t size) {
    if(device == NULL) return -1;
    int _user = device->raw_userdata;
    ew32_dev_t *parent = device->parent;
    int32_t _ret = size;

    if(_user != EW32_STDIN) return 0;

    if(parent != NULL) {
        char *_buf = (char*)data;
        char _n ;
        int32_t _i = 0;

        while (_i < size) {
            _n = getc (stdin);
            if (_n == EOF ) break;
            _buf[_i] = _n;
        };

        _ret = _i;
    } else {
        _ret = ew32_device_read(parent, data, size);
    }

    return _ret;
}

ew32_dev_t __global_std_device[3];


void ew32_dev_std_setdevice(const uint8_t id, ew32_dev_t* parent) {
    if (strcmp(parent->subpath, __global_std_device[id].subpath) == 0)  
        __global_std_device[id].parent = NULL;

    __global_std_device[id].parent = parent;
}

uint8_t ew32_dev_std_setup(const uint8_t id, ew32_dev_t** device) {
    if(device == NULL) return EW32_DEVICE_ERROR_NULL;

    // Instanz initialisieren
    __global_std_device[id].subpath[0] = 's';
    __global_std_device[id].subpath[1] = 't';
    __global_std_device[id].subpath[2] = 'd';

    if(id == EW32_STDOUT)  {
        __global_std_device[id].subpath[3] = 'o';
        __global_std_device[id].subpath[4] = 'u';
        __global_std_device[id].subpath[5] = 't';
    }
    if(id == EW32_STDERR)  {
        __global_std_device[id].subpath[3] = 'e';
        __global_std_device[id].subpath[4] = 'r';
        __global_std_device[id].subpath[5] = 'r';
    }
    if(id == EW32_STDIN)  {
        __global_std_device[id].subpath[3] = 'i';
        __global_std_device[id].subpath[4] = 'n';
    }

    __global_std_device[id].cls = EW32_DEVICE_CLASS_INSTD;
    __global_std_device[id].type = EW32_DEVICE_TYPE_STREAM;
    __global_std_device[id].rw = (id == EW32_STDIN) ? EW32_DEVICE_READ_ONLY : EW32_DEVICE_WRITE_ONLY;
    __global_std_device[id].parent = NULL;
    __global_std_device[id].powerstate = ew32_std_powerstate;
    __global_std_device[id].probe = ew32_std_probe;
    __global_std_device[id].setup = ew32_std_setup;
    __global_std_device[id].write = ew32_std_write;
    __global_std_device[id].read = ew32_std_read;

    __global_std_device[id].raw_userdata = id;
    __global_std_device[id].parent = NULL;

    *device = &__global_std_device[id];
    return EW32_DEVICE_NO_ERROR;
}