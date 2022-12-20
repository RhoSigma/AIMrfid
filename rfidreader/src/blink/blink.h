#include <Arduino.h>

void IRAM_ATTR blinkISR();
void setBlinkState(uint32_t *setBlink, uint32_t *nextBlink);