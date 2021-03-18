#include "contiki.h"
// #include "core/net/rime/rime.h"
#include "random.h"
#include "dev/light-sensor.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "sys/node-id.h"
#include <stdio.h>
#include "Task2/dtw.h"

#define buffSize 8

PROCESS(broadcast_button_process, "Broadcast Button");
AUTOSTART_PROCESSES(&broadcast_button_process);

int IndexCnt=0;
// static float s = 0;
// static int dec;
// static float frac;
static int SendBuf[buffSize];
static int PassBuf[buffSize] = {1, 0, 1, 1, 0, 0, 0, 1};
static bool SendReady = false;


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
        static struct etimer timer, starTime, endTime;
        int u =0;
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
                                

                                SendBuf[IndexCnt] = button_sensor.value(SENSORS_ACTIVE);
                                printf("Btn is: %d to index: %d\n", button_sensor.value(SENSORS_ACTIVE), IndexCnt);
                                

                                //waiting for 3 seconds
                                etimer_set(&timer, CLOCK_SECOND / 4);
                                PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
                                IndexCnt++;
                                if(IndexCnt>=8)
                                {
                                        IndexCnt=0;
                                }
                                SendReady = true;
                                

                        }
                        if(IndexCnt<7)
                        {       
                                //fills rest of buffer with 0
                                for(u=IndexCnt; u<8; u++)
                                {
                                        SendBuf[u] = 0;
                                }
                        }
                        leds_off(LEDS_YELLOW);     //switching off LED yellow after sending
                        IndexCnt=0;
                        etimer_set(&endTime, CLOCK_SECOND);
                        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&endTime));
                        if(SendReady==true)
                        {
                                printf("Whole Sendbuf is currently: %d %d %d %d %d %d %d %d\n", SendBuf[0], SendBuf[1], SendBuf[2], SendBuf[3], SendBuf[4], SendBuf[5], SendBuf[6], SendBuf[7]);
                                //send method
                                double dtw_ = dtw(PassBuf, SendBuf, buffSize, buffSize, buffSize);
                                printf("dtw_ variable is: %lf", dtw_)
                                SendReady=false;
                        }

                        
                }

        PROCESS_END();
}