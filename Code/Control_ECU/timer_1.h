/******************************************************************************
 *
 * Module: timer_1
 *
 * File Name: timer_1.h
 *
 * Description: Header file for the timer_1 driver
 *
 * Author: Omar Saad
 *
 *******************************************************************************/


#ifndef TIMER_1_H_
#define TIMER_1_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/


/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum
{
	NO_CLOCK,NO_PRESCALER,PRESCALER_8,PRESCALER_64,PRESCALER_256,PRESCALER_1024,
	EXTERNAL_FAILLING_PRESCALER,EXTERNAL_RISING_PRESCALER
}Timer1_Prescaler;

typedef enum
{
	NORMAL_MODE,COMPARE_MODE
}Timer1_Mode;

typedef struct {
	uint16 initial_value;
	uint16 compare_value; // it will be used in compare mode only.
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
} Timer1_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Function to initialize timer_1 driver
 * */
void Timer1_init(const Timer1_ConfigType * Config_Ptr);

/*
 * Description :
 * Function to disable timer_1 driver
 * */
void Timer1_deInit(void);

/*
 * Description :
 * Function to set call back function address of timer_1 driver
 * */
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER_1_H_ */
