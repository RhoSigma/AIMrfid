#include <Arduino.h>
#include <config.h>

// forward declarations
extern bool debugVerbose;

extern const char* MCU_TYPE;
extern const char* MCU_ARCH;
extern const char* BUILD_TIME;
extern const char* BUILT_BY;
extern const char* COMMIT_HASH;

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

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

void enableDebugMode()
{
  debugVerbose = true;
  Serial.println(F("Enabled debug mode."));
  Serial.println("MCU_TYPE: " + String(MCU_TYPE));
  Serial.println("MCU_ARCH: " + String(MCU_ARCH));
  Serial.println("HOSTNAME: " + String(HOSTNAME));
  Serial.println("BUILD_TIME: " + String(BUILD_TIME));
  Serial.println("BUILT_BY: " + String(BUILT_BY));
  Serial.println("COMMIT_HASH: " + String(COMMIT_HASH));
}

void retBoardType()
{
  Serial.println(String(MCU_TYPE));
  Serial.println(String(MCU_ARCH));
}