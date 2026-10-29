#pragma once

#include <stdint.h>

typedef ew32_gps_uart_type {
    uint16_t RX;
    uint16_t TX;
    uint16_t baud;
    uint32_t serialconfig;
}  ew32_gps_uart_cfg_t;