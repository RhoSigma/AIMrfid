#include <Arduino.h>
#include <pins.h>
#include <blink/blink.h>

void setBlinkState(uint32_t *setBlink, uint32_t *nextBlink);

uint8_t blinkStep;
uint32_t *blinkPatternPtr;
uint32_t *nextBlinkPatternPtr;\

#ifdef nodemcuv2

void initBlink()
{
  timer1_attachInterrupt(blinkISR); // config timer
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_SINGLE);
  timer1_write(6000); //120000 us
}

void setBlinkState(uint32_t *setBlink, uint32_t *nextBlink)
{
  blinkStep = 0;
  blinkPatternPtr = setBlink;

  if (nextBlink != NULL)
  {
    nextBlinkPatternPtr = nextBlink;
  }

  if (nextBlinkPatternPtr == NULL)
  {
    nextBlinkPatternPtr = blinkPatternPtr;
  }

  blinkISR(); // call to immediately start blink routine
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

#endif

#ifdef az_delivery_devkit_v4

hw_timer_t *blink_timer = NULL;

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


void IRAM_ATTR blinkISR(){ // LED flash

  portENTER_CRITICAL_ISR(&timerMux);

  digitalWrite(LED_PIN, !(blinkStep % 2 == 0)); // invert, led is on is pulled *low*

  timerAlarmWrite(blink_timer, blinkPatternPtr[blinkStep], true);

  blinkStep++;

  if (blinkPatternPtr[blinkStep] == 0)
  {
    blinkStep = 0;
    blinkPatternPtr = nextBlinkPatternPtr;
  }

  portEXIT_CRITICAL_ISR(&timerMux);
}

void initBlink()
{
  blink_timer = timerBegin(1, 256, true);
  timerAttachInterrupt(blink_timer, &blinkISR, true);
  timerAlarmWrite(blink_timer, 1000000, true);
  timerAlarmEnable(blink_timer);
}

void setBlinkState(uint32_t *setBlink, uint32_t *nextBlink)
{
  blinkStep = 0;
  blinkPatternPtr = setBlink;

  if (nextBlink != NULL)
  {
    nextBlinkPatternPtr = nextBlink;
  }

  if (nextBlinkPatternPtr == NULL)
  {
    nextBlinkPatternPtr = blinkPatternPtr;
  }

  blinkISR(); // call to immediately start blink routine
}


#endif