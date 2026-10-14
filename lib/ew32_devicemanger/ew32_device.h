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
#define ES32_DEVICE_CLASS_VIRTUAL      0x00  /**< Virtual or software-emulated device class. */
#define ES32_DEVICE_CLASS_SYSTEM       0x01  /**< Core system or bus controller device class. */
#define ES32_DEVICE_CLASS_EXTERN       0x02  /**< External peripheral or physical hardware device class. */
#define ES32_DEVICE_CLASS_SECONDARY    0x04  /**< Secondary or subordinate device class. */
/// @}

/** @name Device Types */
/// @{
#define EW32_DEVICE_TYPE_INPUT         0X01  /**< Input-only device type. */
#define EW32_DEVICE_TYPE_OUTPUT        0X11  /**< Output-only device type. */
#define EW32_DEVICE_TYPE_INOUT         0X10  /**< Bi-directional input/output device type. */
#define EW32_DEVICE_TYPE_NONE          0X00  /**< Unspecified or generic device type. */
/// @}




#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Core device structure representing any hardware node or virtual entity.
 */
typedef struct ew32device  {
    uint8_t id;                                                     /**< Unique instance identifier or slot index. */
    uint8_t cls;                                                    /**< Device classification (e.g., system, external, virtual). */
    uint8_t type;                                                   /**< Device operational type (input, output, inout). */

    struct ew32device* parent;                                      /**< Pointer to the parent device node (e.g., bus controller). */

    uint8_t (* probe)(struct ew32device* device, void* cfg);         /**< Function pointer for discovering hardware capabilities. */
    uint8_t (* setup)(struct ew32device* device, void* cfg);         /**< Function pointer for initializing the device. */
    uint8_t (* powerstate)(struct ew32device* device, uint8_t state); /**< Function pointer for managing power states. */
    int32_t (* write)(struct ew32device* device, void* data, int32_t size);   /**< Function pointer for writing data or control messages. */
    int32_t (* read)(struct ew32device* device, void* data, int32_t size);    /**< Function pointer for reading data from the device. */

    void* userdata;                                                 /**< Pointer to private driver-specific configuration or state data. */
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