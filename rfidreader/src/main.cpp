/*
 * ----------------------------------------------------------------------
 * RC522 Interfacing with NodeMCU
 * 
 * Pin layout used:
 * ----------------------------------------------------------
 *             MFRC522        Node              Colour
 *             Reader/PCD     MCU      
 * Signal      Pin            Pin      
 * ---------------------------------------------------------
 * RST/Reset   RST            D1  (GPIO5)       Brown
 * SPI SS      SDA(SS)        D2  (GPIO4)       Yellow
 * SPI MOSI    MOSI           D7  (GPIO13)      Blue
 * SPI MISO    MISO           D6  (GPIO12)      Purple
 * SPI SCK     SCK            D5  (GPIO14)      Orange
 * 3.3V        3.3V           3.3V (NEAR USB!)  Red
 * GND         GND            GND  (NEAR USB!)  Black
 * Status LED  VCC (330 ohm)  D4                Grey
 */

#include <main.h>

extern MFRC522::MIFARE_Key RFID_KEY;



void setup() { 
  Serial.begin(9600);
  while(!Serial);

  delay(500); // wait until serial ready
  printSelfInfo();


  SPI.begin(); // Init SPI bus
  initRFID();


  pinMode(LED_PIN, OUTPUT); // config LED pin
  digitalWrite(LED_PIN, LOW);

  // config LED blink pattern
  setBlinkState(slow_blink, slow_blink);

  timer1_attachInterrupt(blinkISR); // config timer
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_SINGLE);
  timer1_write(6000); //120000 us

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(RFID_KEY.keyByte, MFRC522::MF_KEY_SIZE);

  connectWifi();
  setUpUI();
}
 
uint32_t lastUiUpdate;
void loop() {

  if (millis() - lastUiUpdate > UI_UPDATE_MS)
  {
    lastUiUpdate = millis();
    ESPUI.updateLabel(mainLabel, String(lastUiUpdate));
  }

  serviceRFID();

  MDNS.update();
}

