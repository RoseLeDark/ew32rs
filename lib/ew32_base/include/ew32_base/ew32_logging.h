#pragma once

#include <stdint.h>

/* Log-Level Definitionen */
#define EW32_LOGGING_LEVEL_DEBUG    0
#define EW32_LOGGING_LEVEL_INFO     1
#define EW32_LOGGING_LEVEL_WARNING  2
#define EW32_LOGGING_LEVEL_ERROR    3
#define EW32_LOGGING_LEVEL_CRITICAL 4

#define xewLoggingDebug1(tag, text, ARG)    xewLoggingSerial(EW32_LOGGING_LEVEL_DEBUG, (tag), (text), ARG )
#define xewLoggingInfo1(tag, text, ARG )     xewLoggingSerial(EW32_LOGGING_LEVEL_INFO, (tag), (text), ARG )
#define xewLoggingWarning1(tag, text, ARG )  xewLoggingSerial(EW32_LOGGING_LEVEL_WARNING, (tag), (text), ARG )
#define xewLoggingError1(tag, text, ARG )    xewLoggingSerial(EW32_LOGGING_LEVEL_ERROR, (tag), (text), ARG )
#define xewLoggingCritical1(tag, text, ARG ) xewLoggingSerial(EW32_LOGGING_LEVEL_CRITICAL, (tag), (text), ARG )

#define xewLoggingDebug(tag, text)    xewLoggingSerial(EW32_LOGGING_LEVEL_DEBUG, (tag), (text) )
#define xewLoggingInfo(tag, text )     xewLoggingSerial(EW32_LOGGING_LEVEL_INFO, (tag), (text) )
#define xewLoggingWarning(tag, text )  xewLoggingSerial(EW32_LOGGING_LEVEL_WARNING, (tag), (text) )
#define xewLoggingError(tag, text )    xewLoggingSerial(EW32_LOGGING_LEVEL_ERROR, (tag), (text) )
#define xewLoggingCritical(tag, text ) xewLoggingSerial(EW32_LOGGING_LEVEL_CRITICAL, (tag), (text) )

#ifdef __cplusplus
extern "C" {
#endif

typedef int8_t (* logging_handler)(uint8_t level, const char* tag, const char* text);

int8_t xewLoggingInitEx(logging_handler handle, uint8_t minLevel);
/**
 * Zentrale Logging-Funktion.
 * 
 * @param tag   Kategorisierung oder Modul-Name (TAG)
 * @param text  Zu loggender Text (Null-terminiert)
 * @param level Log-Level des Eintrags (z.B. EW32_LOGGING_LEVEL_INFO)
 * @return      0 bei Erfolg, Fehlercode ungleich 0 bei Fehlschlag
 */
int8_t xewLoggingSerial(uint8_t level, const char* tag, const char* text, ...);

/**
 * Setzt das minimale globale Log-Level. Eintrags-Level unterhalb
 * dieses Wertes werden verworfen.
 * 
 * @param level Das minimale auszugebende Log-Level
 * @return      0 bei Erfolg, Fehlercode ungleich 0 bei Fehlschlag
 */
void xewSetLoggingLevel(uint8_t level);

/**
 * Gibt das aktuell aktive minimale Log-Level zurück.
 */
uint8_t xewGetLoggingLevel(void);

#ifdef __cplusplus
}
#endif


