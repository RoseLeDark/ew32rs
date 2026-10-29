/**
 * @file ew32_i2c.h
 * @brief I2C bus driver and communication interface for the ew32rs framework.
 */

#pragma once

#include "ew32_device.h"

#include <stdio.h>

/** @name I2C Bus Event Messages */
/// @{
#if EW32_DEVICE_USE_I2C_AUTOPROBE == 1
#define EW32_BUSI2C_FOUND_NEWDEVICE             10  /**< Event triggered when a new device is detected during bus probe. */
#endif
#define EW32_BUSI2C_RECIVE_DATA                 12  /**< Event triggered when data is received on the bus. */
#define EW32_BUSI2C_CHANGE_STATE                14  /**< Event triggered on bus power or operational state changes. */
/// @}

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Callback function type for handling I2C bus events.
 * 
 * @param sender Pointer to the device sending or triggering the event.
 * @param msg Message type ID (e.g., EW32_BUSI2C_FOUND_NEWDEVICE).
 * @param addrs Pointer to event-specific data (such as an I2C address).
 * @param size Size of the data payload in bytes.
 * @return uint8_t Status code (EW32_DEVICE_NO_ERROR on success).
 */
typedef uint8_t (* i2c_handler)(ew32_dev_t* sender, uint8_t msg, void* addrs, uint32_t size);

/**
 * @brief Configuration structure for the I2C bus driver.
 */
typedef struct es32_i2c_config {
    uint8_t pin_sda;            /**< GPIO pin number assigned for SDA (Serial Data). */
    uint8_t pin_scl;            /**< GPIO pin number assigned for SCL (Serial Clock). */
    uint8_t port_num;           /**< Hardware I2C port controller number. */
    uint8_t address_set[128];   /**< Bitmap representing discovered 7-bit I2C addresses (0-127). */
    uint8_t hardware;           /**< Hardware flags or specific revision identifier. */
    void* apihandler;           /**< Underlying platform-specific driver handle (opaque pointer). */
    i2c_handler handler;        /**< Registered event handler callback for bus notifications. */
} ew32_i2c_cfg_t;

/**
 * @brief Unified I/O messaging structure for read and write operations.
 */
typedef struct ew32_i2c_io {
    uint8_t addr;               /**< Target 7-bit device address, or 255 if targeting the bus itself. */
    uint8_t message_id;         /**< Command or message type identifier. */
    uint32_t timeout;           /**< Operation timeout limit in milliseconds or ticks. */
    uint8_t *reg_addr;          /**< Target register address pointer (used exclusively for read operations). */
    size_t reg_size;            /**< Size of the register address in bytes (used exclusively for read operations). */
    size_t msg_size;            /**< Size of the data message payload buffer. */
    uint8_t* message;              /**< Pointer to the data payload buffer. */
} ew32_i2c_io_t;

/// ========== WRITE STRUCT ==========

/** @name I2C Write Message IDs */
/// @{
#define EW32_I2C_NEWDEV_MSG_ID 1    /**< Message ID to register a newly discovered device. */
/// @}

/**
 * @brief Structure used when registering a new sub-device on the I2C bus.
 */
typedef struct ew32_i2c_registnewdev {
    uint8_t new_addr;                   /**< 7-bit I2C address of the new device. */
    uint32_t scl_speed_hz;              /**< Clock frequency of the SCL line in Hertz. */
    uint32_t scl_wait_us;               /**< Clock stretch timeout or wait time in microseconds. */
    uint32_t disable_ack_check:      1;   /**< Bitfield flag to disable ACK checking if set to 1. */
} ew32_i2c_new_dev_t;

#define EW32_I2C_TRANSMIT_MSG_ID 2      /**< Message ID for raw data transmission. */
// now own struct


/// ========== READ STRUCT ==========

/** @name I2C Read Message IDs */
/// @{
#define EW32_I2C_RECIVE_MSG_ID 125      /**< Message ID for combined transmit/receive read operations. */
/// @}



/**
 * @brief Initializes and allocates an I2C bus device node.
 * 
 * @param sda GPIO pin for SDA.
 * @param scl GPIO pin for SCL.
 * @param port_num Hardware port index.
 * @param handler Callback function for asynchronous bus events.
 * @param device Double pointer to receive the created base device instance.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success, or an error code on failure.
 */
#if EW32_DEVICE_USE_I2C == 1
uint8_t ew32_device_i2c_setup(uint8_t sda, uint8_t scl, uint8_t port_num, i2c_handler handler, ew32_dev_t** device);
uint8_t ew32_device_i2c_isadress(uint8_t port_num, uint8_t address);

int32_t ew32_device_i2c_write(uint8_t port_num,  ew32_i2c_io_t* data, int32_t size);
int32_t ew32_device_i2c_read(uint8_t port_num,  ew32_i2c_io_t* data, int32_t size);
#else
#define ew32_device_i2c_setup(sda, scl, port_num, handler, device)                    
#define ew32_device_i2c_isadress(port_num, address)                                 

#define ew32_device_i2c_write(port_num,  data, size)                                
#define ew32_device_i2c_read(port_num,   data, size)                                
#endif 

#ifdef __cplusplus
}
#endif