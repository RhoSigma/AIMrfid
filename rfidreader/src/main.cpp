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

/*
 * ----------------------------------------------------------------------
 * RC522 Interfacing with ESP32
 * 
 * Pin layout used:
 * ----------------------------------------------------------
 *             MFRC522        Node              Colour
 *             Reader/PCD     MCU      
 * Signal      Pin            Pin      
 * ---------------------------------------------------------
 * SPI MOSI    MOSI           IO23              Blue
 * RST/Reset   RST            IO22              Brown
 * Status LED  VCC (330 ohm)  IO21              Grey
 * GND         GND            GND               Black
 * SPI MISO    MISO           IO19              Purple
 * SPI SCK     SCK            IO18              Orange
 * SPI SS      SDA(SS)        IO5               Yellow
 * 3.3V        3.3V           3.3V              Red
 */

#include <main.h>

extern MFRC522::MIFARE_Key RFID_KEY;



void setup() { 
  Serial.begin(9600);
  while(!Serial);

  delay(500); // wait until serial ready


  SPI.begin(); // Init SPI bus
  initRFID();


  pinMode(LED_PIN, OUTPUT); // config LED pin
  digitalWrite(LED_PIN, LOW);

  initBlink();

  // config LED blink pattern
  setBlinkState(slow_blink, slow_blink);



  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(RFID_KEY.keyByte, MFRC522::MF_KEY_SIZE);

  #ifdef nodemcuv2
    connectWifi();
    setUpUI();
  #endif
}
 
uint32_t lastUiUpdate;
void loop() {


  #ifdef nodemcuv2
    if (millis() - lastUiUpdate > UI_UPDATE_MS)
    {
      lastUiUpdate = millis();
      ESPUI.updateLabel(mainLabel, String(lastUiUpdate));
    }
  #endif

  serviceSerial();

  serviceRFID();

  #ifdef nodemcuv2
    MDNS.update();
  #endif
}

