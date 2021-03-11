// Write a program
// Print a string every 2 seconds
//  including a counter that goes up at every iteration


#include "contiki.h"
#include <stdio.h>
#include "dev/button-sensor.h"
/*---------------------------------------------------------------------------*/
/* Defined in C++ code */

/*---------------------------------------------------------------------------*/
PROCESS(test_button_process, "test process");
PROCESS(counter_process, "counting up or down");
AUTOSTART_PROCESSES(&test_button_process,&counter_process);

/*---------------------------------------------------------------------------*/

static int8_t incThis = 0;

PROCESS_THREAD(test_button_process, ev, data)
{

	PROCESS_BEGIN();
	SENSORS_ACTIVATE(button_sensor);
	
	
	
	while(1) {
		PROCESS_WAIT_EVENT_UNTIL(ev==sensors_event && data == &button_sensor);
		printf("button pressed");
		if (incThis == 0){
			incThis++;
			printf("increasing Value");}
		else{
			incThis=0;
			printf("Decreasing Value");
		}
			}
	PROCESS_END();
}


PROCESS_THREAD(counter_process, ev, data){
	static struct etimer timer;
	static int counter;
	PROCESS_BEGIN();
	
	etimer_set(&timer, CLOCK_SECOND);
	
	while(1){
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
		
		printf("counter %i", counter=counter+incThis);
		if(incThis>=0){
			printf("--> and up \n");
		}
		else{
		printf("<--and down \n");
		}
		etimer_reset(&timer);
	}
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/