/**
 * @file ew32_device.h
 * @brief Core device abstraction layer and interface definitions for the ew32rs framework.
 */

#pragma once

#include "ew32_error.h"
#include "ew32_config.h"
#include "stdint.h"

/** @name Device Classes */
/// @{
#define ES32_DEVICE_CLASS_VIRTUAL   0x00  /**< Virtual or software-emulated device class. */
#define ES32_DEVICE_CLASS_BUS       0x01 
#define ES32_DEVICE_CLASS_SECONDARY 0x02
#define ES32_DEVICE_CLASS_NETWORK   0x07
#define ES32_DEVICE_CLASS_PROTOCOL  0x08
#define ES32_DEVICE_CLASS_GPIO      0x09

/// @}

/** @name Device Types */
/// @{
/** @name Device Types */
/// @{
#define EW32_DEVICE_TYPE_UNKNOWN                 0x0000
#define EW32_DEVICE_TYPE_I2C                     0x0001
#define EW32_DEVICE_TYPE_SPI                     0x0002
#define EW32_DEVICE_TYPE_UART                    0x0003
#define EW32_DEVICE_TYPE_CONTROLLER              0x0004
#define EW32_DEVICE_TYPE_DISK                    0x0005
#define EW32_DEVICE_TYPE_INPORT_PORT             0x0006
#define EW32_DEVICE_TYPE_KEYBOARD                0x0007
#define EW32_DEVICE_TYPE_MIDI_IN                 0x0008
#define EW32_DEVICE_TYPE_MIDI_OUT                0x0009
#define EW32_DEVICE_TYPE_MOUSE                   0x000A
#define EW32_DEVICE_TYPE_ETH                     0x000B
#define EW32_DEVICE_TYPE_NULL                    0x000C
#define EW32_DEVICE_TYPE_PARALLELPORT            0x000D
#define EW32_DEVICE_TYPE_WIFI                    0x000E
#define EW32_DEVICE_TYPE_PRINTER                 0x000F
#define EW32_DEVICE_TYPE_SCANNER                 0x0010
#define EW32_DEVICE_TYPE_SERIALPORT              0x0011
#define EW32_DEVICE_TYPE_SCREEN                  0x0012
#define EW32_DEVICE_TYPE_SOUND                   0x0013
#define EW32_DEVICE_TYPE_STREAM                  0x0014
#define EW32_DEVICE_TYPE_TAPE                    0x0015
#define EW32_DEVICE_TYPE_TRANSPORT               0x0016
#define EW32_DEVICE_TYPE_VIDEO                   0x0017
#define EW32_DEVICE_TYPE_AUDIOOUT                0x0018
#define EW32_DEVICE_TYPE_AUDIOIN                 0x0019
#define EW32_DEVICE_TYPE_I2S                     0x001A
#define EW32_DEVICE_TYPE_ONE_WIRE                0x001B
#define EW32_DEVICE_TYPE_BATTERY                 0x001C
#define EW32_DEVICE_TYPE_BUS_EXTENDER            0x001D
#define EW32_DEVICE_TYPE_MODEM                   0x001E
#define EW32_DEVICE_TYPE_RS485                   0x001F
#define EW32_DEVICE_TYPE_STORAGE                 0x0020
#define EW32_DEVICE_TYPE_SMARTCARD               0x0021
#define EW32_DEVICE_TYPE_CRYPT                   0x0022
#define EW32_DEVICE_TYPE_BLUETOOTH               0x0023
#define EW32_DEVICE_TYPE_BIOMETRIC               0x0024
#define EW32_DEVICE_TYPE_DEVAPI                  0x0025
#define EW32_DEVICE_TYPE_CONSOLE                 0x0026
#define EW32_DEVICE_TYPE_SYSENV                  0x0027
#define EW32_DEVICE_TYPE_VBLOCK                  0x0028
#define EW32_DEVICE_TYPE_STORAGE_REPLICATION     0x0029
#define EW32_DEVICE_TYPE_TRUSTENV                0x002A
#define EW32_DEVICE_TYPE_PMEM                    0x002B
#define EW32_DEVICE_TYPE_HARDWARE_ACCELERATOR    0x002C


/* --- Embedded & Robotik Sensoren / Aktoren --- */
#define EW32_DEVICE_TYPE_GPS                     0x002D
#define EW32_DEVICE_TYPE_LIGHT_SENSOR            0x002E
#define EW32_DEVICE_TYPE_TEMP_SENSOR             0x002F
#define EW32_DEVICE_TYPE_HUMIDITY_SENSOR         0x0030
#define EW32_DEVICE_TYPE_PRESSURE_SENSOR         0x0031
#define EW32_DEVICE_TYPE_IMU                     0x0032
#define EW32_DEVICE_TYPE_DISTANCE_SENSOR         0x0033
#define EW32_DEVICE_TYPE_GAS_SENSOR              0x0034
#define EW32_DEVICE_TYPE_MOTOR                   0x0035
#define EW32_DEVICE_TYPE_SERVO                   0x0036
#define EW32_DEVICE_TYPE_RELAY                   0x0037

#define EW32_DEVICE_TYPE_POWERMANAGER            0x0100

#define EW32_DEVICE_TYPE_USER                    0x1000
#define EW32_DEVICE_TYPE_MAX                     0x1FFF
/// @}


#define EW32_DEVICE_READ_ONLY                    0x01
#define EW32_DEVICE_WRITE_ONLY                   0x10
#define EW32_DEVICE_READ_WRITE                   0x11

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Core device structure representing any hardware node or virtual entity.
 */
typedef struct ew32device  {
    char    subpath[16];
    ew32_file_t id;                                                     /**< Unique instance identifier or slot index. */
    uint16_t cls;                                                    /**< Device classification (e.g., system, external, virtual). */
    uint16_t type;                                                   /**< Device operational type (input, output, inout). */
    uint8_t  rw;                                               /**< Sread write stats 0 => r. 1 w . 2 rw*/
    struct ew32device* parent;                                      /**< Pointer to the parent device node (e.g., bus controller). */

    uint8_t (* probe)(struct ew32device* device, void* cfg);         /**< Function pointer for discovering hardware capabilities. */
    uint8_t (* setup)(struct ew32device* device, void* cfg);         /**< Function pointer for initializing the device. */
    uint8_t (* powerstate)(struct ew32device* device, uint8_t state); /**< Function pointer for managing power states. */
    int32_t (* write)(struct ew32device* device, void* data, int32_t size);   /**< Function pointer for writing data or control messages. */
    int32_t (* read)(struct ew32device* device, void* data, int32_t size);    /**< Function pointer for reading data from the device. */

    union  {
        void*       userdata;                                                 /**< Pointer to private driver-specific configuration or state data. */
        uint32_t    raw_userdata;
    };
    
    
} ew32_dev_t;

/**
 * @brief Sends data or a control command to a device.
 * 
 * @param device Pointer to the target device node.
 * @param data Pointer to the payload buffer or message structure.
 * @param size Size of the payload data in bytes.
 * @return int32_t Number of bytes written, or a negative error code on failure.
 */
int32_t ew32_device_send(ew32_dev_t* device, void* data, int32_t size);

/**
 * @brief Reads data from a device.
 * 
 * @param device Pointer to the target device node.
 * @param data Pointer to the destination buffer.
 * @param size Maximum number of bytes to read.
 * @return int32_t Number of bytes read, or a negative error code on failure.
 */
int32_t ew32_device_read(ew32_dev_t* device, void* data, int32_t size);

/**
 * @brief Sets or queries the power state of a device.
 * 
 * @param device Pointer to the target device node.
 * @param state Target power state value.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success, or an error code.
 */
uint8_t ew32_device_pstate(ew32_dev_t* device, uint8_t state);

/**
 * @brief Executes the probe routine on a device to detect underlying hardware.
 * 
 * @param device Pointer to the target device node.
 * @param cfg Optional configuration pointer passed to the probe function.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success, or an error code.
 */
uint8_t ew32_device_probe(ew32_dev_t* device, void* cfg);

/**
 * @brief Executes the setup routine to initialize a device for operation.
 * 
 * @param device Pointer to the target device node.
 * @param cfg Optional configuration pointer passed to the setup function.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success, or an error code.
 */
uint8_t ew32_device_setup(ew32_dev_t* device, void* cfg);

/**
 * @brief Assigns private user data or driver context to a device.
 * 
 * @param device Pointer to the target device node.
 * @param data Pointer to the context structure to attach.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success.
 */
uint8_t ew32_device_set_userdata(ew32_dev_t* device, void* data);

/**
 * @brief Retrieves the private user data or driver context from a device.
 * 
 * @param device Pointer to the target device node.
 * @param data Double pointer to receive the stored context pointer.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success, or an error code if null.
 */
uint8_t ew32_device_get_userdata(ew32_dev_t* device, void** data);

/**
 * @brief Checks whether a device is currently capable of sending/writing data.
 * 
 * @param device Pointer to the target device node.
 * @return uint8_t EW32_DEVICE_YES if capable, otherwise EW32_DEVICE_NO.
 */
uint8_t ew32_device_can_send(ew32_dev_t* device);

/**
 * @brief Checks whether a device is currently capable of receiving/reading data.
 * 
 * @param device Pointer to the target device node.
 * @return uint8_t EW32_DEVICE_YES if capable, otherwise EW32_DEVICE_NO.
 */
uint8_t ew32_device_can_read(ew32_dev_t* device);

/**
 * @brief Gets the parent device node associated with this device.
 * 
 * @param device Pointer to the target device node.
 * @param parent Double pointer to receive the parent device reference.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success, or an error code.
 */
uint8_t ew32_device_get_parent(ew32_dev_t* device, ew32_dev_t** parent);

/**
 * @brief Sets or updates the parent device node for this device.
 * 
 * @param device Pointer to the target device node.
 * @param parent Pointer to the new parent device node.
 * @return uint8_t EW32_DEVICE_NO_ERROR on success.
 */
uint8_t ew32_device_set_parent(ew32_dev_t* device, ew32_dev_t* parent);

#ifdef __cplusplus
}
#endif