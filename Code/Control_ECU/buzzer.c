 /******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: Source file for the Buzzer driver
 *
 * Author: Omar Saad
 *
 *******************************************************************************/

#include "buzzer.h"
#include "gpio.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "avr/io.h" /* To use the IO Ports Registers */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function to Initialize the Buzzer
 * - Setup the direction for the buzzer pin as output pin through the GPIO driver
 * - Turn off the buzzer through the GPIO
 */
void Buzzer_init()
{
	GPIO_setupPinDirection(Buzzer_PORT_ID, Buzzer_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(Buzzer_PORT_ID, Buzzer_PIN_ID, LOGIC_LOW);
}

/*
 * Description :
 * Function to enable the Buzzer through the GPIO.
 */
void Buzzer_on(void)
{
	GPIO_writePin(Buzzer_PORT_ID, Buzzer_PIN_ID, LOGIC_HIGH);
}

/*
 * Description :
 * Function to disable the Buzzer through the GPIO.
 */
void Buzzer_off(void)
{
	GPIO_writePin(Buzzer_PORT_ID, Buzzer_PIN_ID, LOGIC_LOW);
}

