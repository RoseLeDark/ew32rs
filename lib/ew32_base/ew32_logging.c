#include "ew32_base/ew32_logging.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

logging_handler ___handle = NULL;
uint8_t __minLevel = 0;


int8_t xewLoggingInitEx(logging_handler handle, uint8_t minLevel) {
    ___handle = handle;
    __minLevel = minLevel;
    return 0;
}
int8_t xewLoggingSerial(uint8_t level, const char* tag, const char* text, ...) {
    if(level < __minLevel) return 0;

    char loc_buf[64];
    char * temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, text);
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(loc_buf), text, copy);
    va_end(copy);
    if(len < 0) {
        va_end(arg);
        return 0;
    }
    if(len >= (int)sizeof(loc_buf)){ 
        temp = (char*) malloc(len+1);
        if(temp == NULL) {
            va_end(arg);
            return 0;
        }
        len = vsnprintf(temp, len+1, text, arg);
    }
    va_end(arg);

    if(___handle) return ___handle(level, tag, temp);

    if(temp != loc_buf) {
        free(temp);
    }

    return 0;
}

void xewSetLoggingLevel(uint8_t level) {
    __minLevel = level;
}

uint8_t xewGetLoggingLevel(void) {
    return __minLevel;
}