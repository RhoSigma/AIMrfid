#include <Arduino.h>
#include <pins.h>
#include <blink/blink.h>

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

void IRAM_ATTR blinkISR(){ // LED flash

  digitalWrite(LED_PIN, !(blinkStep % 2 == 0)); // invert, led is on is pulled *low*

  timer1_write(blinkPatternPtr[blinkStep]);

  blinkStep++;

  if (blinkPatternPtr[blinkStep] == 0)
  {
    blinkStep = 0;
    blinkPatternPtr = nextBlinkPatternPtr;
  }
}