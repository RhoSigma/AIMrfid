#include <Arduino.h>

void printSelfInfo();
void printHex(byte *buffer, byte bufferSize);
void printDec(byte *buffer, byte bufferSize);
uint64_t getDeltaMillis(uint64_t compareTime);