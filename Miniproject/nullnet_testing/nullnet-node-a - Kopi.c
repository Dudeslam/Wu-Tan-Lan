/*
 * Copyright (c) 2017, RISE SICS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         NullNet unicast example
 * \author
*         Simon Duquennoy <simon.duquennoy@ri.se>
 *
 */

#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "random.h"

#include <string.h>
#include <stdio.h> /* For printf() */

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Configuration */
#define SEND_INTERVAL (60 * CLOCK_SECOND)
static linkaddr_t dest_addr =         {{ 0x02, 0x02, 0x02, 0x00, 0x02, 0x74, 0x12, 0x00 }};

#if MAC_CONF_WITH_TSCH
#include "net/mac/tsch/tsch.h"
static linkaddr_t coordinator_addr =  {{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};
#endif /* MAC_CONF_WITH_TSCH */

uint8_t count[8] = {0};
uint8_t count2[8] = {0};
int i = 0;
unsigned rec = 0;
unsigned first_rec = 0;


void print_received_data(uint8_t data)
{	
	if(data != 0x00) {
	  uint8_t temperature;
	  uint8_t receive_nr;
	  temperature = data & 0x3f;
      receive_nr = ((data & 0xC0) >> 6) + 2;
	  printf("Received temperature %u from ", temperature);
	  printf("%u\n", receive_nr);
	}
}

int next_count(int inc)
{
	if(inc > 22) {
	  inc = 0;
	  NETSTACK_NETWORK.output(&dest_addr);
	}
    else
	  inc++;
    return inc;
}

/*---------------------------------------------------------------------------*/
PROCESS(nullnet_example_process, "NullNet unicast example");
AUTOSTART_PROCESSES(&nullnet_example_process);

int counter =0;

/*---------------------------------------------------------------------------*/
void input_callback(const void *data, uint16_t len,
  const linkaddr_t *src, const linkaddr_t *dest)
{
	if(rec == 0) {
		memcpy(&count, data, sizeof(count));
		rec = 1;
	}
	else {
		memcpy(&count2, data, sizeof(count));
		rec = 2;
	}
	first_rec = 1;
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(nullnet_example_process, ev, data)
{
	
  static struct etimer periodic_timer;
  static uint8_t temp[24] = {0};
  //int tempo = 0;

  PROCESS_BEGIN();

#if MAC_CONF_WITH_TSCH
  tsch_set_coordinator(linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr));
#endif /* MAC_CONF_WITH_TSCH */

  /* Initialize NullNet */
  nullnet_buf = (uint8_t *)&temp;
  nullnet_len = sizeof(temp);
  nullnet_set_input_callback(input_callback);

  if(!linkaddr_cmp(&dest_addr, &linkaddr_node_addr)) {
    etimer_set(&periodic_timer, SEND_INTERVAL);
    while(1) {
	
	  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
		
	  if(rec == 2) {
		//if((24 - counter) > 7) {
		for (i = 0; i < 8; i++) {
		  memcpy(&temp[counter], &count[i], sizeof(count));
		  counter = next_count(counter);
		  printf("Counter: %d\n", counter);
		}
		
		for (i = 0; i < 8; i++) {
		  memcpy(&temp[counter], &count2[i], sizeof(count));
		  counter = next_count(counter);
		  printf("Counter: %d\n", counter); 
		}
		rec = 0;
		printf("Rec:%u\n", rec);
	  }
	  else if(rec == 1) {
		for (i = 0; i < 8; i++) {
		  memcpy(&temp[counter], &count[i], sizeof(count));
		  counter = next_count(counter);
		  printf("Counter: %d\n", counter);
		}
		rec = 0;
	  }
	  if(first_rec == 1) {
		  temp[counter] = 10 + (random_rand() % 5);
		  temp[counter] = temp[counter] | 0x80; // Adding sender nr to extract at sink.
		  counter = next_count(counter);
		  
		  printf("Counter: %d\n", counter);
	  }
      etimer_reset(&periodic_timer);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
