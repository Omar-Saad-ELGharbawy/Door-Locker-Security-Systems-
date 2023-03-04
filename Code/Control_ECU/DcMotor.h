/******************************************************************************
 *
 * Module: Dc_Motor
 *
 * File Name: DcMotor.h
 *
 * Description: Header file for the DC_Motor driver
 *
 * Author: Omar Saad
 *******************************************************************************/

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "std_types.h"
#include "gpio.h"
#include "pwm.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define DC_Motor_IN1_PORT_ID                 PORTB_ID
#define DC_Motor_IN1_PIN_ID                  PIN0_ID   /*IN1 connected to PB0 in PORTB */

#define DC_Motor_IN2_PORT_ID                 PORTB_ID
#define DC_Motor_IN2_PIN_ID                  PIN1_ID   /*IN2 connected to PB1 in PORTB */

#define DC_Motor_EN1_PORT_ID                 PORTB_ID
#define DC_Motor_EN1_PIN_ID                  PIN3_ID   /*EN1 connected to PB3(OC0) in PORTB */

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	STOP,CW,A_CW
}DcMotor_State;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize pin directions of the motor
 * Stop the motor at the beginning
 */
void DcMotor_Init(void);

/*
 * Description :
 * Function responsible for rotate the DC Motor CW/ or A-CW or stop
 * Send the required duty cycle to the PWM driver based on the required speed value.
 */
void DcMotor_Rotate(DcMotor_State state);


#endif /* DCMOTOR_H_ */
