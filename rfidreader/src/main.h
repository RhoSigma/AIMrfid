#include "Arduino.h"
#include <EEPROM.h>
#ifdef nodemcuv2

    #include <ESPUI.h>
    #include <ESP8266WiFi.h>
    #include <ESP8266mDNS.h>
    #include <wifiInterface/wifiInterface.h>

#endif

#include <wifiInterface/webui.h>

#include <SPI.h>
#include <rfid/rfid.h>
#include <MFRC522.h>

#include <pins.h>
#include <board.h>
#include <config.h>

#include <blinkPatterns.h>
#include <blink/blink.h>
#include <utils/utils.h>
#include <serial/serial.h>

#include <buildID.h>

bool debugVerbose;