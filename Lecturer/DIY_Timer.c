// Write a program
// Print a string every 2 seconds
//  including a counter that goes up at every iteration

#include "contiki.h"
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/* Defined in C++ code */
uint8_t wrapper_function(void);
/*---------------------------------------------------------------------------*/
PROCESS(node_process, "Node process");
AUTOSTART_PROCESSES(&node_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
	int incThis = 0;
	static struct etimer timer; // Define timer variable
	PROCESS_BEGIN();
	etimer_set(&timer, 2); // Initiate timer to trigger event
	while(1) {
		printf("Hello, world\n");
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer)); // Wait until time is expired
		etimer_reset(&timer); // Reset timer
		incThis++;
			}
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/