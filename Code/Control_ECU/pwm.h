/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.h
 *
 * Description: Header file for the PWM driver
 *
 * Author: Omar Saad
 *
 *******************************************************************************/

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description:
 * Generate a PWM signal with Duty Cycle
 * Duty Cycle can be changed passing its value to the function
 */
void PWM_Timer0_Start(uint8 duty_cycle);


#endif /* PWM_H_ */
