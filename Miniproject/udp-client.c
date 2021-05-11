#include "contiki.h"
#include "net/routing/routing.h"
#include "random.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "stdio.h"
#include "stdlib.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY  1
#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

#define SEND_INTERVAL		  (60 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;
static float get_temp()
{
  static int val;
  static float s = 0;

  val = sht11_sensor.value(SHT11_SENSOR_TEMP);
  if(val != -1)
  {
    s= ((0.01*val) - 39.60);
    return s;
  }
  return 0;
}

static float get_light()
{
  static int val;
  static float s = 0;

  val = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
  if(val != -1)
  {
    s = (float)(val * 0.4071);
    return s;
  }
  return 0;
}

/*---------------------------------------------------------------------------*/
PROCESS(udp_client_process, "UDP client");
AUTOSTART_PROCESSES(&udp_client_process);
/*---------------------------------------------------------------------------*/
static void
udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{

  LOG_INFO("Received response '%.*s' from ", datalen, (char *) data);
  LOG_INFO_6ADDR(sender_addr);
#if LLSEC802154_CONF_ENABLED
  LOG_INFO_(" LLSEC LV:%d", uipbuf_get_attr(UIPBUF_ATTR_LLSEC_LEVEL));
#endif
  LOG_INFO_("\n");

}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{
  static struct etimer periodic_timer;
  static unsigned count;
  static char str[32];
  uip_ipaddr_t dest_ipaddr;
  uint8_t temp, light;
  uint32_t ClockNow;
  float frac;
  int A;

  int UseSens = 0;

  PROCESS_BEGIN();
  clock_init();

  PROCESS_BEGIN();

  /* Initialize UDP connection */
  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
                      UDP_SERVER_PORT, udp_rx_callback);

  etimer_set(&periodic_timer, random_rand() % SEND_INTERVAL);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    if(NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {
      /* Send to DAG root */
      if(UseSens == 0)
      {
        temp = 20 + (random_rand() % 15);
        light = 30 + (random_rand() % 50)
      }
      else
      {
        temp = get_temp();
        light = get_light();
      }
      
      
      LOG_INFO("Sending temp %u to ", temp);
      LOG_INFO_6ADDR(&dest_ipaddr);
      LOG_INFO_("\n");
      ClockNow = clock_time()/128 ;
      A=ClockNow;
      frac=(ClockNow-A)*100;
      // LOG_INFO("Estimated time used %d.%02u milliseconds\n", A,(unsigned int)frac);
      snprintf(str, sizeof(str), "Current Temp %u to time %d.%02u", temp, A, (unsigned int)frac);
      LOG_INFO("Sending request %u to ", count);
      LOG_INFO_6ADDR(&dest_ipaddr);
      LOG_INFO_("\n");
      snprintf(str, sizeof(str), "hello %d", count);

      simple_udp_sendto(&udp_conn, str, strlen(str), &dest_ipaddr);
      count++;
    } else {
      LOG_INFO("Not reachable yet\n");
    }

    /* Add some jitter */
    etimer_set(&periodic_timer, SEND_INTERVAL
      - CLOCK_SECOND + (random_rand() % (2 * CLOCK_SECOND)));
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
