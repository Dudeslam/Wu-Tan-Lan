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

PROCESS(rssiSIG, "RSSI reading");
AUTOSTART_PROCESSES(&rssiSIG);

#define LOG_MODULE "Test"
#define LOG_LEVEL LOG_LEVEL_INFO


PROCESS_THREAD(rssiSIG, ev, data)
{
    printf("Starting RSSI process\n");
    clock_init();
	
	int rssi = 0;
	int channel = 0;
    int i = 0;
    float rssiMean = 0;
    int samples = 300;
    
    static struct etimer starTime;

        PROCESS_BEGIN();

            SENSORS_ACTIVATE(button_sensor);
            SENSORS_ACTIVATE(light_sensor);
			
			cc2420_on();
            
            while(1)
            {
				for (channel = 11; channel<27; channel++)
                {
                    rssiMean = 0;
                    for(i = 0; i < samples; i++)
                    {
                        cc2420_set_channel(channel);
                        rssi = cc2420_rssi();
                        rssiMean += rssi;
                    }
                    rssiMean = rssiMean/samples;
                    int A;
                    float frac;
                    A=rssiMean; // get integral part, i.e. A=3
                    frac=(rssiMean-A)*100; // get fractional part, i.e. frac=55
                    printf("RSSI mean value: %d.%02u for channel %d\n", A, (unsigned int)frac, channel);
                }
                etimer_set(&starTime, CLOCK_SECOND*60);
                PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&starTime));
            }
        
		cc2420_off();
        SENSORS_DEACTIVATE(button_sensor);
        SENSORS_DEACTIVATE(light_sensor);
        PROCESS_END();

}