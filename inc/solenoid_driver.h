/*
 * solenoid_driver.h
 *
 *  Created on: Apr 1, 2017
 *      Author: abates
 */

#ifndef SOLENOID_DRIVER_H_
#define SOLENOID_DRIVER_H_

#include "stm32f0xx.h"

#define SOLENOID_TIME_ON_MS		(200)

extern const int duty_cycle[11];

#define LEFT_SOLENOID_CCR		(TIM2->CCR3)
#define RIGHT_SOLENOID_CCR		(TIM2->CCR4)

typedef enum {
	left_solenoid,
	right_solenoid
}Solenoids;

extern void solenoid_driver_init(void);
extern void turn_on_solenoid(Solenoids solenoid, uint16_t dutyCyclePercent10000);

#endif /* SOLENOID_DRIVER_H_ */
