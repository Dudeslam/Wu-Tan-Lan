#include "contiki.h"
#include "random.h"
#include "dev/light-sensor.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "sys/node-id.h"
#include <stdio.h>
#include "dtw.h"
#include "string.h"

#define buffSize 8

PROCESS(broadcast_button_process, "Broadcast Button");
AUTOSTART_PROCESSES(&broadcast_button_process);

int IndexCnt=0;
// static float s = 0;
// static int dec;
// static float frac;
static double SendBuf[buffSize];
static double PassBuf[buffSize] = {1, 0, 1, 1, 0, 0, 0, 1};
static bool SendReady = false;
static int CastBuf[buffSize];

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
        static struct etimer timer, starTime, endTime, ledTime;
        int u, button =0;
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
                                // leds_off(LEDS_ALL);
                                if(IndexCnt==0)
                                {
                                        etimer_set(&starTime, CLOCK_SECOND);
                                        printf("You can now type\n");
                                }
                                leds_on(LEDS_YELLOW);      //switching on the YELLOW LED after type sequence started
                                
                                if(button_sensor.value(SENSORS_ACTIVE)==1)
                                { 
                                        button = 0;
                                }
                                else
                                {
                                        button = 1;
                                }

                                SendBuf[IndexCnt] = button;
                                printf("Btn is: %d to index: %d\n", button, IndexCnt);
                                

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
                                
                                memcpy(&CastBuf, &SendBuf, buffSize);
                                printf("Whole Sendbuf is currently: %d %d %d %d %d %d %d %d\n", CastBuf[0], CastBuf[1], CastBuf[2], CastBuf[3], CastBuf[4], CastBuf[5], CastBuf[6], CastBuf[7]);
                                //send method
                                double dtw_ = dtw(PassBuf, SendBuf, buffSize, buffSize, buffSize);
                                int A;
                                float frac;
                                A=dtw_;
                                frac=(dtw_ -A )*100;
                                printf("dtw_ variable is: %d.%02u\n", A,(unsigned int)frac);
                                if(dtw_ <= 2)
                                {
                                        leds_on(LEDS_GREEN);
                                }
                                else
                                {
                                        leds_on(LEDS_RED);
                                }
                                SendReady=false;

                        }
                        //turn off all LEDS after 2 sec
                        etimer_set(&ledTime, CLOCK_SECOND*3);
                        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&ledTime));
                        leds_off(LEDS_GREEN);
                        leds_off(LEDS_RED);
                        
                }
        SENSORS_DEACTIVATE(button_sensor);
        SENSORS_DEACTIVATE(light_sensor);
        PROCESS_END();
}