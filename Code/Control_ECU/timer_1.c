/******************************************************************************
 *
 * Module: timer_1
 *
 * File Name: timer_1.c
 *
 * Description: Source file for the timer_1 driver
 *
 * Author: Omar Saad
 *
 *******************************************************************************/

#include "timer_1.h"
#include "gpio.h" /* To use GPIO Driver */
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "avr/io.h" /* To use the IO Ports Registers */
#include <avr/interrupt.h> /* For timer_1 ISR */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
/* Interrupt for Normal (Overflow) Mode */
ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}
/* Interrupt for Compare Mode */
ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize timer_1 driver
 * */
void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{

	/* for non-PWM mode SET FOC1A,FOC1B bits in TCCR1A Register */
	SET_BIT(TCCR1A,FOC1A);
	SET_BIT(TCCR1A,FOC1B);
	/* Initial Value for Timer1 */
	TCNT1 = Config_Ptr->initial_value;
	/*
	 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
	 * of TCCR1B Register
	 */
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler);

	if((Config_Ptr->mode)==NORMAL_MODE)
	{

		/* Enable Timer1 overflow interrupts */
		SET_BIT(TIMSK,TOIE1);
		/* Enable overflow flag */
		SET_BIT(TIFR,TOV1);
	}
	else if((Config_Ptr->mode)==COMPARE_MODE)
	{

		/* Configure timer control register TCCR1B
		 * CTC Mode WGM12=1 WGM13=0
		 */
		SET_BIT(TCCR1B,WGM12);

		/*Set the compare value for compare mode */
		OCR1A=Config_Ptr->compare_value;

		/* Enable Timer1 Compare A Interrupt */
		SET_BIT(TIMSK,OCIE1A);
		/* Enable CTC flag */
		SET_BIT(TIFR,OCF1A);
	}
}

/*
 * Description :
 * Function to disable timer_1 driver
 * */
void Timer1_deInit(void)
{
	/*Clear all timer register*/
	TCCR1A=0;
	TCNT1=0;
	/*Disable the interrupt MISK register*/
	CLEAR_BIT(TIMSK,TOIE1);
	CLEAR_BIT(TIMSK,OCIE1A);
	g_callBackPtr = NULL_PTR;
}

/*
 * Description :
 * Function to set call back function address of timer_1 driver
 * */
void Timer1_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;

}

