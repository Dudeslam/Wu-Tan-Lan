/*
* Print sensor readings every second
*/

#include "contiki.h"

#include "dev/light-sensor.h"
//#include "dev/sht11/sht11-sensor.h"

#include <stdio.h>
#include <math.h>

/*---------------------------------------------------------------------------*/
PROCESS(sensor_acq_process,"Sensor Acquisition");
AUTOSTART_PROCESSES(&sensor_acq_process);
/*---------------------------------------------------------------------------*/
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

static float get_light2()
{
  static int val;
  static float s = 0;

  val = light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC);
  if(val != -1)
  {
    s = (float)(val * 0.4071);
    return s;
  }
  return 0;
}

/*static float get_humid()
{
  static int val;
  static float s = 0;

  val=sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
  if(val != -1){
   s= (((0.0405*val) - 4) + ((-2.8 * 0.000001)*(pow(val,2))));
    return s;
  }
  return 0;
}
*/
//static float get_temp()
//{
//  static int val;
//  static float s = 0;

//  val = sht11_sensor.value(SHT11_SENSOR_TEMP);
//  if(val != -1)
//  {
//    s= ((0.01*val) - 39.60);
//    return s;
//  }
//  return 0;
//}
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(sensor_acq_process,ev,data)
{
      static struct etimer et;
      //static int val;
      static float s = 0;
      static int dec;
      static float frac;

      PROCESS_BEGIN();

      printf("Starting Sensor Example.\n");

      SENSORS_ACTIVATE(light_sensor);
      //SENSORS_ACTIVATE(sht11_sensor);

      while(1)
      {
	   etimer_set(&et, CLOCK_SECOND * 1);


	   PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        /*  s = get_temp();
          dec = s;
          frac = s - dec;
          printf("\nTemperature=%d.%02u C (%d)\n", dec, (unsigned int)(frac * 100),val);*/

    	    //s = get_humid();
    	  	//dec = s;
    	  	//frac = s - dec;
    	  	//printf("Humidity=%d.%02u %% (%d)\n", dec, (unsigned int)(frac * 100),val);

          s = get_light();
          dec = s;
          frac = s-dec;
          printf("Light TOTAL =%d.%02uf lux \n", dec, (unsigned int)(frac*100));

          s = get_light2();
          dec = s;
          frac = s-dec;
          printf("Light PHOTO =%d.%02uf lux \n", dec, (unsigned int)(frac*100));

          etimer_reset(&et);

      } //end of while

      SENSORS_DEACTIVATE(light_sensor);
    //  SENSORS_DEACTIVATE(sht11_sensor);

      PROCESS_END();
}