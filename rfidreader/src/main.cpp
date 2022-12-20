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

 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];



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

  timer1_attachInterrupt(blinkISR); // config timer
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_SINGLE);
  timer1_write(6000); //120000 us

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

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

  MDNS.update();

}

