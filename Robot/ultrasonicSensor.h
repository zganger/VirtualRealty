/*
 * ultrasonicSensor.h
 *
 *  Created on: Feb 16, 2016
 *      Author: Mlkromis
 */

#ifndef SOURCES_ULTRASONICSENSOR_H_
#define SOURCES_ULTRASONICSENSOR_H_

#include "PE_LDD.h"

/*
 * \brief Called in case of an overflow during input capture. This function is called from an interrupt!
 * \param UserDataPtr Pointer to user structure
 */
void US_EventEchoOverflow(LDD_TUserData *UserDataPtr);

/*
 * \brief Called by the input capture interrupt for a raising or falling edge of the Echo pin
 * \param UserDataPtr Pointer to user structure
 */
void US_EventEchoCapture(LDD_TUserData *UserDataPtr);

/*
 * \brief Convert measured microseconds to centimeters.
 * \param[in] temperatureCelsius Air temperature in degree Celsius
 */
int US_usToCentimeters(int microseconds, int temperatureCelsius);


int US_Measure_us(void);

void Measure(void);
/*
 * \brief Driver initialization routine.
 */
void US_Init(void);

#endif /* SOURCES_ULTRASONICSENSOR_H_ */
