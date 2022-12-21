#include <Arduino.h>

//                                    .             .             .               -              -              -             .             .             .        end
uint32_t sos_blink[19] = {30000, 50000, 30000, 50000, 30000, 100000, 100000, 50000, 100000, 50000, 100000, 50000, 30000, 50000, 30000, 50000, 30000, 600000, 0};
uint32_t slow_blink[3] = {10000, 600000, 0}; // slow, looks like 'ready' or 'standby'

uint32_t three_blinks[7] = {40000, 30000, 40000, 30000, 40000, 200000, 0};


// 100000 medium-slow flash, looks like error
// 60000 medium flash
// 30000 fast flash
// 15000 data blink, very fast

// 600000 off, 60000 on = ready? USB connection maybe
