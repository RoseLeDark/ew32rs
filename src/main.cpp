#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <Wire.h>

#include "ew32_devm/ew32_api.h"
#include "ew32_base/ew32_logging.h"
#include "ew32_devm/ew32_devm.h"


// GPS module connections
#define     RXPin       33
#define     TXPin       32
#define     GPSBaud  9600

//HardwareSerial                      __gpsSerial(2);
TinyGPSPlus                         __gps;

/*
1:I2C] Found device 0x19  // LIS3DHTR Seeed_Arduino_LIS3DHTR-master xyz
[1:I2C] Found device 0x29 // TSL2591 https://github.com/adafruit/Adafruit_TSL2591_Library light
[1:I2C] Found device 0x60 // Adafruit_MotorShield
[1:I2C] Found device 0x70
[1:I2C] Found device 0x77 // BMP085 https://github.com/adafruit/Adafruit_BMP085_Unified -- bmp280 barometer
*/

// Temperature&Humidity Sensor(DHT20)
// SDA 33 21
// SCL 36 22

int8_t __logging_handler(uint8_t level, const char* tag, const char* text) {
   Serial.printf("[%d:%s] %s\n", level, tag, text); return 0;
}

void setup() {
   Serial.begin(115200);
   xewLoggingInitEx(&__logging_handler, EW32_LOGGING_LEVEL_DEBUG );

   // Start the software serial port at the GPS's default baud
   //__gpsSerial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);;

   ew32_tasksystem_init();
   ew32_devicemanager_create();

   
}

String __lat = "0.000000";
String __lot = "0.000000";
String __sats = "0";
bool __dirty = true;

static unsigned long __previousMillis = 0; 

void loop(void) {
   unsigned long curticks = millis();

 //  while (__gpsSerial.available() > 0) {
 //   char t = __gpsSerial.read();
  //    __gps.encode(t);

      //Serial.print(t);
   


  /* if (__gps.location.isValid()) {
      __dirty = true;
      __lat = String(__gps.location.lat(), 6);
      __lot = String(__gps.location.lng(), 6);
      __sats = String(__gps.satellites.value());
   } else {
      __lat = String("No Data");
      __lot = String("No Data");
      __sats = String("No Data");
   }

   if(__dirty) {
        Serial.printf("Lat: %s\n", __lat.begin());
        Serial.printf("Lng: %s\n", __lot.begin());
        Serial.printf("Sat: %s\n", __sats.begin());

      __dirty = false;
   }
   delay(500);*/
}

uint8_t ew32_powsermanager_system_test(uint8_t curstate) {
   return 5;
}
