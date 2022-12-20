#include <Arduino.h>

//                                    .             .             .               -             -             .             .             .        end
uint32_t sos_blink[18] = {30000, 50000, 30000, 50000, 30000, 100000, 100000, 50000, 100000, 50000, 30000, 50000, 30000, 50000, 30000, 600000, 0};
uint32_t slow_blink[3] = {60000, 600000, 0}; // slow, looks like 'ready' or 'standby'


// 100000 medium-slow flash, looks like error
// 60000 medium flash
// 30000 fast flash
// 15000 data blink, very fast

// 600000 off, 60000 on = ready? USB connection maybe
