#include <stdio.h>
#include <math.h>
#include "contiki.h"
#include "random.h"
#include "dev/light-sensor.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "sys/node-id.h"
#include "sys/log.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>

PROCESS(DCTSig, "DCT signal thingy");
AUTOSTART_PROCESSES(&DCTSig);

#define LOG_MODULE "Test"
#define LOG_LEVEL LOG_LEVEL_INFO

PROCESS_THREAD(DCTSig, ev, data)
{
    printf("Starting DCT process\n");
    clock_init();
    
    // static struct etimer startTime;

        PROCESS_BEGIN();

            SENSORS_ACTIVATE(button_sensor);
            SENSORS_ACTIVATE(light_sensor);
            
            int x = 0;
            while(1)
            {
                LOG_INFO("Entered while loop\n");
                printf("x: %d\n", x);
                x += 1;
            }
        SENSORS_DEACTIVATE(button_sensor);
        SENSORS_DEACTIVATE(light_sensor);
        PROCESS_END();

}