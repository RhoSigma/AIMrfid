#include <MFRC522.h>
#include <pins.h>
#include <config.h>

// forward declarations
extern void printHex(byte *buffer, byte bufferSize);
extern void printDec(byte *buffer, byte bufferSize);
extern void setBlinkState(uint32_t *setBlink, uint32_t *nextBlink);
extern uint64_t getDeltaMillis(uint64_t compareTime);

extern uint32_t five_blinks[];    // used on card rejection
extern uint32_t one_blink_long[]; // used on valid read

MFRC522::MIFARE_Key RFID_KEY;
MFRC522 rfid(SS_PIN, RST_PIN);
byte lastUID[4];

uint64_t lastRead;


// called on debounced card read
void onValidRead()
{
  Serial.println(F("Valid card read."));

  setBlinkState(one_blink_long, NULL); // indicate card read, return to last blink pattern

  // Store current UID into lastUID array
  for (byte i = 0; i < 4; i++) { lastUID[i] = rfid.uid.uidByte[i]; }

  // -----------------------------------------------------------------
  // @Owen: take it from here

  Serial.println(F("The NUID tag is:"));
  Serial.print(F("In hex: "));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  Serial.print(F("In dec: "));
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();

  // -----------------------------------------------------------------

}

// convenience function, called on debounce rejection
void onRejectDebounce()
{
  setBlinkState(five_blinks, NULL);
  Serial.println(F("Rejected card read bounce."));
}

void initRFID()
{
  rfid.PCD_Init();
}

bool isMIFARE(MFRC522::PICC_Type piccType)
{
  if 
  (
    piccType == MFRC522::PICC_TYPE_MIFARE_MINI ||  
    piccType == MFRC522::PICC_TYPE_MIFARE_1K ||
    piccType == MFRC522::PICC_TYPE_MIFARE_4K
  ) 
  { return true;}
  return false;
}

bool isReadDebounced()
{
  // compare current to last card read
  if 
  (
    rfid.uid.uidByte[0] == lastUID[0] &&
    rfid.uid.uidByte[1] == lastUID[1] &&
    rfid.uid.uidByte[2] == lastUID[2] &&
    rfid.uid.uidByte[3] == lastUID[3] 
  ) 
  {
    // card already read
    if (getDeltaMillis(lastRead) < CARD_DEBOUNCE_TIMEOUT_MS)
    {
      // card has already been read within debounce threshold
      lastRead = millis();
      onRejectDebounce();
      return false;
    }
  }

  // card is unique or read after debounce threshold
  lastRead = millis();
  return true;
}

// called from main loop
void serviceRFID()
{


  // TODO: unsure of this, refactor?
  for (byte i = 0; i < 6; i++) {
      RFID_KEY.keyByte[i] = 0xFF;
  }

  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the current UID has been read
  if ( ! rfid.PICC_ReadCardSerial())
    return;


  // TODO: refactor? do we need this?
  // Serial.print(F("PICC type: "));
  // MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (!isMIFARE(rfid.PICC_GetType(rfid.uid.sak)))
  {
    return; // not miFARE
  }

  if (isReadDebounced())
  {
    // valid card read
    onValidRead();
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}