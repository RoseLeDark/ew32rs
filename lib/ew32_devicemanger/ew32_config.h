/**
 * @file ew32_config.h
 * @brief Global framework configuration and build-time options for ew32rs.
 */

#pragma once

/** @name Device Manager Allocation Modes */
/// @{
#define EW32_STATIC                             0    /**< Static allocation mode using fixed compile-time arrays. */
#define EW32_DYNAMIC                            1    /**< Dynamic allocation mode (if supported). */
/// @}

/** @name Configuration State Flags */
/// @{
#define EW32_CONFIG_YES                         1    /**< Affirmative state flag. */
#define EW32_CONFIG_NO                          0    /**< Negative state flag. */
/// @}

/** @name Target Platform Identifiers */
/// @{
#define EW32_PLATFORM_ESP                       1    /**< Espressif ESP-IDF platform target. */
#define EW32_PLATFORM_ARDUINO                   2    /**< Arduino framework target. */
#define EW32_PLATFORM_STM32                     3    /**< STM32 microcontroller target. */
/// @}

/// ================        CONFIG START        ================

/** @brief Maximum number of supported devices on the I2C bus. */
#ifndef EW32_DEVICE_I2C_MAX_DEVICES
    #define EW32_DEVICE_I2C_MAX_DEVICES         4
#endif

/** @brief Operating mode for the device manager subsystem. */
#ifndef EW32_DEVICE_MANAGER_MODE
    #define EW32_DEVICE_MANAGER_MODE EW32_STATIC
#endif

/** @brief Enable or disable I2C bus subsystem support. */
#ifndef EW32_DEVICE_USE_I2C   
    #define EW32_DEVICE_USE_I2C                 EW32_CONFIG_YES
#endif   

/** @brief Enable or disable SPI bus subsystem support. */
#ifndef EW32_DEVICE_USE_SPI   
    #define EW32_DEVICE_USE_SPI                 EW32_CONFIG_YES
#endif  

/** @brief Enable or disable I2S audio bus subsystem support. */
#ifndef EW32_DEVICE_USE_I2S   
    #define EW32_DEVICE_USE_I2S                 EW32_CONFIG_NO
#endif  

/** @brief Enable or disable GPIO pin subsystem support. */
#ifndef EW32_DEVICE_USE_PIN   
    #define EW32_DEVICE_USE_PIN                 EW32_CONFIG_YES
#endif  

/** @brief Enable or disable Bluetooth subsystem support. */
#ifndef EW32_DEVICE_USE_BT  
    #define EW32_DEVICE_USE_BT                  EW32_CONFIG_YES
#endif 

/** @brief Enable or disable Wi-Fi network subsystem support. */
#ifndef EW32_DEVICE_USE_WIFI 
    #define EW32_DEVICE_USE_WIFI                EW32_CONFIG_NO
#endif  

/** @brief Enable or disable GPS sensor subsystem support. */
#ifndef EW32_DEVICE_USE_GPS
    #define EW32_DEVICE_USE_GPS                 EW32_CONFIG_YES
#endif  

#if EW32_DEVICE_USE_GPS == EW32_CONFIG_YES
    /** @brief Enable periodic system time synchronization from GPS data. */
    #ifndef EW32_DEVICE_USE_GPS_CLOCK
        #define EW32_DEVICE_USE_GPS_CLOCK       EW32_CONFIG_YES
    #endif 
#endif

#if EW32_DEVICE_USE_I2C == EW32_CONFIG_YES
    /** @brief Enable automatic bus probing during I2C initialization. */
    #ifndef EW32_DEVICE_USE_I2C_AUTOPROBE
        #define EW32_DEVICE_USE_I2C_AUTOPROBE   EW32_CONFIG_YES
    #endif 
#endif

#if EW32_DEVICE_MANAGER_MODE == EW32_STATIC
    /** @brief Maximum total capacity of statically allocated device entries. */
    #ifndef EW32_DEVICE_MANAGER_DEVICES
        #define EW32_DEVICE_MANAGER_DEVICES     16
    #endif
#endif

/// ================ DO NOT EDIT AFTER THIS LINE ================

#if ESP_PLATFORM == 1
#define EW32_PLATFORM   EW32_PLATFORM_ESP
#endif 