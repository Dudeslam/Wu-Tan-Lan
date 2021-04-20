#include <stdio.h>
#include <math.h>
#include "contiki.h"
#include "net/mac/tsch/tsch.h"
#include "net/netstack.h"
#include "dev/radio.h"
#include "random.h"
#include "dev/light-sensor.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "sys/node-id.h"
#include "sys/log.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include "arch/dev/radio/cc2420/cc2420.h"

PROCESS(rssiSIG, "DCT signal thingy");
AUTOSTART_PROCESSES(&rssiSIG);

#define LOG_MODULE "Test"
#define LOG_LEVEL LOG_LEVEL_INFO


PROCESS_THREAD(rssiSIG, ev, data)
{
    printf("Starting RSSI process\n");
    clock_init();
	
	int rssi = 0;
	int channel = 0;
    
    static struct etimer starTime;
    

        PROCESS_BEGIN();

            SENSORS_ACTIVATE(button_sensor);
            SENSORS_ACTIVATE(light_sensor);
			
			int cc2420_on();
            			
            while(1)
            {
				
				for(channel = 11; channel<27; channel++)
				{
					cc2420_set_channel(channel);
					rssi = cc2420_rssi();
					printf("The RSSI value for channel %d is: %d\n", channel, rssi);
				}
				
				etimer_set(&starTime, CLOCK_SECOND*30);
				PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&starTime));
            }
		int cc2420_off();
        SENSORS_DEACTIVATE(button_sensor);
        SENSORS_DEACTIVATE(light_sensor);
        PROCESS_END();

}