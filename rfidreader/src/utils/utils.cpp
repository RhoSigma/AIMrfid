#include <Arduino.h>
#include <config.h>


void printSelfInfo()
{
  Serial.println(MCU_TYPE);
  Serial.println("Firmware build: ");
}

uint64_t deltaTime;
uint64_t getDeltaMillis(uint64_t compareTime)
{
  if (compareTime > millis())
  {
    // millis() overflowed
    // calculate real delta
    deltaTime = UINT64_MAX - compareTime;

    // will overflow again
    if (millis() > UINT64_MAX - deltaTime)
    {
      return UINT64_MAX; // delta between input and cur is greater than UINT64_MAX
    }
    deltaTime += millis();
    return deltaTime;
  }
  return millis() - compareTime;
}


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