/*
 * ultrasonicSensor.c
 *
 *  Created on: Feb 16, 2016
 *      Author: Mlkromis
 */

#include "ultrasonicSensor.h"
#include "TU1.h"

/*
 * ultrasonicSensor.c
 *
 *  Created on: Feb 15, 2016
 *      Author: Mlkromis
 */
 typedef enum {
    ECHO_IDLE, /* device not used */
    ECHO_TRIGGERED, /* started trigger pulse */
    ECHO_MEASURE, /* measuring echo pulse */
    ECHO_OVERFLOW, /* measurement took too long */
    ECHO_FINISHED /* measurement finished */
  } US_EchoState;

  typedef struct {
    LDD_TDeviceData *echoDevice; /* input capture device handle (echo pin) */
    volatile US_EchoState state; /* state */
    TU1_TValueType capture; /* input capture value */
  } US_DeviceType;

  static US_DeviceType usDevice; /* device handle for the ultrasonic device */

  void US_Init(void) {
    usDevice.state = ECHO_IDLE;
    usDevice.capture = 0;
    usDevice.echoDevice = TU1_Init(&usDevice);
  }

  void US_EventEchoOverflow(LDD_TUserData *UserDataPtr) {
		US_DeviceType *ptr = (US_DeviceType*)UserDataPtr;
    ptr->state = ECHO_OVERFLOW;
  }

  void US_EventEchoCapture(LDD_TUserData *UserDataPtr){
    US_DeviceType *ptr = (US_DeviceType*)UserDataPtr;
    if (ptr->state==ECHO_TRIGGERED) { /* 1st edge, this is the raising edge, start measurement */
    	//LED_SetVal();
    	TU1_ResetCounter(ptr->echoDevice);
      ptr->state = ECHO_MEASURE;
    } else if (ptr->state==ECHO_MEASURE) { /* 2nd edge, this is the falling edge: use measurement */
      (void)TU1_GetCaptureValue(ptr->echoDevice, 0, &ptr->capture);
      ptr->state = ECHO_FINISHED;
    }

  }

int US_Measure_us(void) {
    int us;

    /* send 10us pulse on TRIG line. */
    TRIG_SetVal();
    WAIT1_Waitus(10);
    TRIG_ClrVal();
    usDevice.state = ECHO_TRIGGERED;
    while(usDevice.state!=ECHO_FINISHED) {
      /* measure echo pulse */
      if (usDevice.state==ECHO_OVERFLOW) { /* measurement took too long? */
        usDevice.state = ECHO_IDLE;
        return 0; /* no echo, error case */
      }
    }
    us = (usDevice.capture*1000UL)/(TU1_CNT_INP_FREQ_U_0/1000);
    return us;
  }

  int calcAirspeed_dms(uint8_t temperatureCelsius) {
    /* Return the airspeed depending on the temperature, in deci-meter per second */
    unsigned int airspeed; /* decimeters per second */

    airspeed = 3313 + (6 * temperatureCelsius); /* dry air, 0% humidity, see http://en.wikipedia.org/wiki/Speed_of_sound */
    airspeed -= (airspeed/100)*15; /* factor in ~15% for a relative humidity of ~40% */
    return airspeed;
  }

  int US_usToCentimeters(int microseconds, int temperatureCelsius) {
    return (microseconds*100UL)/calcAirspeed_dms(temperatureCelsius)/2; /* 2 because of two way */
  }

  void Measure(void) {
    int us, cm;
    int buf[8];

    us = US_Measure_us();
    cm = US_usToCentimeters(us, 22);
    if(cm < 10){

	   //LED_SetVal();

    }
  }


