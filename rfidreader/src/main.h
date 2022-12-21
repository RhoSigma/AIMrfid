#include "Arduino.h"
#include <EEPROM.h>
#include <ESPUI.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <wifiInterface/wifiInterface.h>
#include <wifiInterface/webui.h>

#include <SPI.h>
#include <rfid/rfid.h>

#include <pins.h>
#include <config.h>

#include <blinkPatterns.h>
#include <blink/blink.h>
#include <utils/utils.h>