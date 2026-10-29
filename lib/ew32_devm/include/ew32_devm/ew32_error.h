#pragma once


/** @name Error and Status Codes */
/// @{
#define EW32_DEVICE_NO_ERROR            0    /**< Operation completed successfully with no errors. */
#define EW32_DEVICE_ERROR_NULL          1    /**< Error: Null pointer encountered or invalid argument. */
#define EW32_DEVICE_ERROR_NOTIMPL       2    /**< Error: Requested operation or function is not implemented. */
#define EW32_DEVICE_ERROR_NOT_SUPPORTED 3    /**< Error: Operation or feature is not supported by this device. */
#define EW32_DEVICE_ERROR_WAIT          4    /**< Error/State: Operation timed out or requires waiting. */
#define EW32_DEVICE_ERROR_BUSY          5    /**< Error/State: Device or resource is currently busy. */
#define EW32_DEVICE_ERROR_INVALID_ARG   6
#define EW32_DEVICE_ERROR_NOT_FOUND     7
#define EW32_DEVICE_ERROR_NOT_USED      8
#define EW32_DEVICE_ERROR_ALLREADY_INIT 9

#define EW32_DEVICEMANAGER_FULL         1
/// @}