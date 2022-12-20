/*
 * ----------------------------------------------------------------------
 * Example program showing how to read new NUID from a PICC to serial.
 * ----------------------------------------------------------------------
 * https://circuits4you.com
 * 
 * RC522 Interfacing with NodeMCU
 * 
 * Typical pin layout used:
 * ----------------------------------------------------------
 *             MFRC522      Node                Colour
 *             Reader/PCD   MCU      
 * Signal      Pin          Pin      
 * ---------------------------------------------------------
 * RST/Reset   RST          D1  (GPIO5)       Brown
 * SPI SS      SDA(SS)      D2  (GPIO4)       Yellow
 * SPI MOSI    MOSI         D7  (GPIO13)      Blue
 * SPI MISO    MISO         D6  (GPIO12)      Purple
 * SPI SCK     SCK          D5  (GPIO14)      Orange
 * 3.3V        3.3V         3.3V (NEAR USB!)  Red
 * GND         GND          GND  (NEAR USB!)  Black
 * Status LED  --           D4                Grey (330 ohm)
 */

#include <Arduino.h>
#include <EEPROM.h>
#include <ESPUI.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <wifiInterface/wifiInterface.h>

#include <SPI.h>
#include <MFRC522.h>

#include <pins.h>
#include <config.h>
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

//                                    .             .             .               -             -             .             .             .        end
uint32_t sos_blink[18] = {30000, 50000, 30000, 50000, 30000, 100000, 100000, 50000, 100000, 50000, 30000, 50000, 30000, 50000, 30000, 600000, 0};
uint32_t slow_blink[3] = {60000, 600000, 0}; // slow, looks like 'ready' or 'standby'



// 100000 medium-slow flash, looks like error
// 60000 medium flash
// 30000 fast flash
// 15000 data blink, very fast

// 600000 off, 60000 on = ready? USB connection maybe


uint8_t blinkStep;
uint32_t *blinkPatternPtr;
uint32_t *nextBlinkPatternPtr;

void setBlinkState(uint32_t *setBlink, uint32_t *nextBlink)
{
  blinkStep = 0;
  blinkPatternPtr = setBlink;

  if (nextBlink == NULL)
  {
    nextBlinkPatternPtr = blinkPatternPtr;
    return;
  }
  nextBlinkPatternPtr = nextBlink;
}

void IRAM_ATTR onTimerISR(){ // LED flash

  digitalWrite(LED_PIN, !(blinkStep % 2 == 0)); // invert, led is on is pulled *low*

  timer1_write(blinkPatternPtr[blinkStep]);

  blinkStep++;

  if (blinkPatternPtr[blinkStep] == 0)
  {
    blinkStep = 0;
    blinkPatternPtr = nextBlinkPatternPtr;
  }
}

bool serialConnected;


void setup() { 
  Serial.begin(9600);
  while(!Serial);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  pinMode(LED_PIN, OUTPUT); // config LED pin
  digitalWrite(LED_PIN, LOW);

  // config LED blink pattern
  setBlinkState(sos_blink, NULL);

  timer1_attachInterrupt(onTimerISR); // config timer
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_SINGLE);
  timer1_write(6000); //120000 us

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

  connectWifi();
  setUpUI();
}
 
void loop() {

  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
  }
  else Serial.println(F("Card read previously."));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

