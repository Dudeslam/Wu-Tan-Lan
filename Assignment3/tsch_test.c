#include "contiki.h"
#include "net/mac/tsch/tsch.h"
#include "net/netstack.h"
#include "dev/radio.h"
#include "tsch_test.h"
 
/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "TSCH Stats"
#define LOG_LEVEL LOG_LEVEL_MAC
 
/*---------------------------------------------------------------------------*/
#if TSCH_STATS_ON
/*---------------------------------------------------------------------------*/
struct tsch_global_stats tsch_stats;
struct tsch_neighbor_stats tsch_neighbor_stats;
/* Called every TSCH_STATS_DECAY_INTERVAL ticks */
static struct ctimer periodic_timer;
static void periodic(void *);
/*---------------------------------------------------------------------------*/
void tsch_stats_init(void)
{    
#if TSCH_STATS_SAMPLE_NOISE_RSSI
   int i;
 
   for(i = 0; i < TSCH_STATS_NUM_CHANNELS; ++i) {
     tsch_stats.noise_rssi[i] = TSCH_STATS_DEFAULT_RSSI;
     tsch_stats.channel_free_ewma[i] = TSCH_STATS_DEFAULT_CHANNEL_FREE;
   }
 #endif
 
   tsch_stats_reset_neighbor_stats();
 
   /* Start the periodic processing soonish */
   ctimer_set(&periodic_timer, TSCH_STATS_DECAY_INTERVAL / 10, periodic, NULL);
}