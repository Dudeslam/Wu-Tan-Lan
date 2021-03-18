#include "contiki.h"
// #include "core/net/rime/rime.h"
#include "random.h"
#include "dev/light-sensor.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "sys/node-id.h"
#include <stdio.h>
#define buffSize 8

PROCESS(broadcast_button_process, "Broadcast Button");
AUTOSTART_PROCESSES(&broadcast_button_process);

int IndexCnt=0;
// static float s = 0;
// static int dec;
// static float frac;
static int SendBuf[buffSize];

static void printer(int buf[])
{
         size_t n = sizeof(buf);
        printf("Whole Sendbuf with length %d is currently: ", n);
        int x;
        for(x=0; x<n;x++)
        {
                printf("%d ", buf[x]);
        }
        printf("\n");

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

PROCESS_THREAD(broadcast_button_process, ev, data)
{
        static struct etimer timer, starTime;

        PROCESS_BEGIN();

                SENSORS_ACTIVATE(button_sensor);
                SENSORS_ACTIVATE(light_sensor);


                while(1)   //always waiting for a button click
                {
                        // s = get_light();
                        // dec = s;
                        // frac = s-dec;
                        // printf("Light TOTAL =%d.%02uf lux \n", dec, (unsigned int)(frac*100));
                        while(get_light()<70)
                        {
                                if(IndexCnt==0)
                                {
                                        etimer_set(&starTime, CLOCK_SECOND);
                                        printf("You can now type\n");
                                }
                                leds_on(LEDS_YELLOW);      //switching on the YELLOW LED after type sequence started
                                // PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);

                                SendBuf[IndexCnt] = button_sensor.value(SENSORS_ACTIVE);
                                printf("%d\n", button_sensor.value(SENSORS_ACTIVE));


                                //waiting for 3 seconds
                                etimer_set(&timer, CLOCK_SECOND * 3);
                                PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
                                IndexCnt++;
                                if(IndexCnt>=8)
                                {
                                        IndexCnt=0;
                                }


                        }
                        if(IndexCnt<7)
                        {
                                int u;
                                for(u=IndexCnt; u=7; u++)
                                {
                                        SendBuf[u] = 0;
                                }
                        }
                        leds_off(LEDS_YELLOW);     //switching off the red LED after 5 seconds
                        IndexCnt=0;
                        printer(SendBuf);
                        //print function for sendbuf
                }

        PROCESS_END();
}