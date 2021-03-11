/*
 * Program toggling through the different LEDs every second
 */

#include "contiki.h"
#include <stdio.h>
#include "dev/leds.h"
#include "leds.h"
#include "stdlib.h"
#include <math.h>
#include "dev/light-sensor.h"
#include <stdbool.h>

/*---------------------------------------------------------------------------*/
PROCESS(Sink_receiver, "Secret knock Sequence");
AUTOSTART_PROCESSES(&Sink_receiver);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(Sink_receiver, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    static uint8_t ALL = 0x70;

    leds_off(ALL);

    
    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 2);
        //
        uint8_t VAL = 0x10;
        //if dtw match
            //turn on/off red LED
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
            leds_toggle(VAL);
            printf("LED RED %u\n", VAL);
            etimer_reset(&timer);
        //else
            //turn on/off green LED
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
            leds_toggle(VAL=VAL<<1);
            printf("LED GREEN (yellow) %u\n", VAL);
            etimer_reset(&timer);
    }
PROCESS_END();
}
/*---------------------------------------------------------------------------*/