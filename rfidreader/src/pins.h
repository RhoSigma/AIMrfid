#ifndef pins_h
    #define pins_h 1

    #include "Arduino.h"

    #ifdef nodemcuv2
        constexpr uint8_t LED_PIN = LED_BUILTIN;
        constexpr uint8_t RST_PIN = 5;
        constexpr uint8_t SS_PIN = 4;
    #endif

    #ifdef az_delivery_devkit_v4
        constexpr uint8_t LED_PIN = 21;
        constexpr uint8_t RST_PIN = 22;
        constexpr uint8_t SS_PIN = 5;
    #endif


#endif
