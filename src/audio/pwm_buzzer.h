/*
 * Starter code for ECE 266 Lab 7, Sound Player (PWM), fall 2024
 *
 * pwmbuzzer.c: PWM buzzer functions
 *
 * Created by Zhao Zhang
 */

#ifndef PWM_BUZZER_H_
#define PWM_BUZZER_H_

#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_timer.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>

/*
 * Initialize the timer PWM functions connected to the buzzer
 */
void PwmBuzzerInit();



/*
 * Set the buzzer parameters
 */
void PwmBuzzerSet(pwmPeriod, pwmWidth);

#endif /* PWD_BUZZER_H_ */
