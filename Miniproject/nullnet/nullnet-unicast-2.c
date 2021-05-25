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
#define SEND_INTERVAL (2 * CLOCK_SECOND)
static linkaddr_t dest_addr =         {{ 0x01, 0x01, 0x01, 0x00, 0x01, 0x74, 0x12, 0x00 }};

#if MAC_CONF_WITH_TSCH
#include "net/mac/tsch/tsch.h"
static linkaddr_t coordinator_addr =  {{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};
#endif /* MAC_CONF_WITH_TSCH */

unsigned dummy = 0;
unsigned rec = 0;
unsigned rec_from = 0;

/*---------------------------------------------------------------------------*/
PROCESS(nullnet_example_process, "NullNet unicast example");
AUTOSTART_PROCESSES(&nullnet_example_process);

/*---------------------------------------------------------------------------*/
void input_callback(const void *data, uint16_t len,
  const linkaddr_t *src, const linkaddr_t *dest)
{
  // Hvis mere data skal modtages her skal if'en ændres
  if(len == sizeof(unsigned)) {
    // unsigned count;
    // memcpy(&count, data, sizeof(count));
    // printf("Size of count: %d\n", sizeof(count));
	  // memcpy(&dummy, data, sizeof(count));
	  // count = count & 0x003f;
    // LOG_INFO("Received %u from ", count);
    // LOG_INFO_LLADDR(src);
    // LOG_INFO_("\n");
	  // rec = 1;
    // -------------------      Der er en fejl når vi udskriver data fra node 3, men kun fordi jeg har tilføjet så der tages imod 2 temperature fra nr 4
    unsigned long long int count;
    memcpy(&count, data, sizeof(count));
    memcpy(&dummy, data, sizeof(count));
    // printf("sizeof dummy: %d\n", sizeof(dummy));
    // printf("sizeof count: %d\n", sizeof(count));
    // printf("\ncallback: received count: 0x%llX\n", count);
    printf("callback: received dummy: 0x%X\n", dummy);
    unsigned flag;
    flag = count & 0xC000;
    memcpy(&rec_from, &flag, sizeof(flag));
    flag = flag >> 14;
    flag = flag + 2;
    printf("flag: %d\n", flag);
    unsigned count_1;
    count_1 = count & 0x3f00;
    count_1 = count_1 >> 8;
    unsigned count_2;
    count_2 = count & 0x003f;
    LOG_INFO("Received %u & %u from ", count_1, count_2);
    LOG_INFO_LLADDR(src);
    LOG_INFO_("\n");
    rec = 1;
	
  }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(nullnet_example_process, ev, data)
{
  static struct etimer periodic_timer;
  static unsigned long long int count = 0;
  static unsigned temp_count1 = 0;
  static unsigned temp_count2 = 0;
  static unsigned temp = 0;
  static unsigned rec_temp = 0;
  static unsigned recFrom4 = 0;
  static unsigned flag = 0;
  static unsigned flag2 = 0;
  static unsigned dataFrom3 = 0;

  PROCESS_BEGIN();

#if MAC_CONF_WITH_TSCH
  tsch_set_coordinator(linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr));
#endif /* MAC_CONF_WITH_TSCH */

  /* Initialize NullNet */
  nullnet_buf = (uint8_t *)&count;
  nullnet_len = sizeof(count);
  nullnet_set_input_callback(input_callback);

  if(!linkaddr_cmp(&dest_addr, &linkaddr_node_addr)) {
    etimer_set(&periodic_timer, SEND_INTERVAL);
    while(1) {
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
	  
      temp = 20 + (random_rand() % 15);
      rec_temp = 1;
      flag = rec_from;
      printf("rec_from = 0x%X\n", flag); // Kan helt sikkert laves bedre
      if(flag == 0x8000) {
        // Received data from #4
        // Send the data, including data from #3 if some
        recFrom4 = 1; // enables sending
      } else {
        // Received data from #3
        // Store the data in a buffer, wait to receive from #4
        memcpy(&dataFrom3, &dummy, sizeof(unsigned)); // only one byte needed
        printf("Storing data 0x%X from 3\n", dataFrom3);
        recFrom4 = 0;
      }

      
      if(rec == 1 && rec_temp == 1 && recFrom4 == 1) {
        memcpy(&count, &dummy, sizeof(count));
        dummy = 0;
        printf("count: 0x%llX\n", count);
        printf("bit shifted count: 0x%llX\n", (count << 48)); // kan det være en løsning til at nulstille data?
        temp_count1 = (count >> 8) & 0x3f;
        temp_count2 = count & 0x003f;
        flag2 = count & 0x00C0;
        flag2 = flag2 >> 14;
        flag2 = flag2 + 2;

        printf("Appending 0x%X from 3 to payload\n", dataFrom3); // X is for printing in hex
        printf("count before dataFrom3 is appended: 0x%llX\n", count); 
        count = (count << 8) + dataFrom3; // Appends 0x00 in the first message if node 4 is sending before node 3
        // Prøv at ændre (count << 8) til (count >> 8), kan måske du sammen med det i linje 163
        printf("count after dataFrom3 is appended:  0x%llX\n", count);

        LOG_INFO("Sending %u & %u ", temp_count1, temp_count2);
        printf("and %u to ", temp);
        LOG_INFO_LLADDR(&dest_addr);
        LOG_INFO_("\n");

        printf("count before temp is appended: 0x%llX\n", count);
        count = (count << 8) + temp;
        printf("count after temp is appended: 0x%llX\n", count);
        
        printf("count to send to 1: 0x%llX\n", count);
        NETSTACK_NETWORK.output(&dest_addr);
        // Nulstiller variabler, igen ikke bedste løsning
        rec = 0;
        rec_temp = 0;
        recFrom4 = 0;
        dataFrom3 = 0; // clear data from node 3
        count = 0; // clear data from node 4
      } else {
        printf("Node 4 has not sent yet\n");
      }

        etimer_reset(&periodic_timer);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
