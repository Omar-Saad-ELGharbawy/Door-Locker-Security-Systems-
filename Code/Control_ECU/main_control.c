/******************************************************************************
 *
 * File Name: Control_main.c
 *
 * Description: main file of Control ECU
 *
 * Author: Omar Saad
 *
 *******************************************************************************/
#include "timer_1.h"
#include "buzzer.h"
#include "twi.h"
#include "DcMotor.h"
#include "external_eeprom.h"
#include "uart.h"
#include "avr/io.h" /* To use the SREG Register */
#include <util/delay.h> /* For the delay functions */

#define MC1_READY 0x1F
#define MC2_READY 0x10

/*******************************************************************************
 *                      Global Variables                                   *
 *******************************************************************************/

/* Arrays to store first and second passwords*/
uint8 g_first_received_password[5];
uint8 g_second_received_password[5];
uint8 g_password[5];
uint8 eeprom_password[5];
uint8 key;

/* g_timerCount for timer interrupt call back function */
uint8 g_timerCount = 0;
/* g_step for program cases */
uint8 g_step = 1;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

boolean passwords_is_equal(uint8 password_1[],uint8 password_2[])
{
	boolean flag;
	for (uint8 i=0;i<5;i++)
	{
		if (password_1[i]==password_2[i])
		{
			flag = TRUE;
		}
		else
		{
			flag = FALSE;
			break;
		}
	}
	return flag;
}

void save_password_in_eeprom(uint8 * password)
{
	for (uint8 i=0;i<5;i++)
	{
		EEPROM_writeByte(0x0311+i, password[i]); /* Write 0x0F in the external EEPROM */
		_delay_ms(10);
	}
}

void read_password_from_eeprom(uint8 * password)
{
	for (uint8 i=0;i<5;i++)
	{
		EEPROM_readByte(0x0311+i, &password[i]);  /* Read 0x0F from the external EEPROM */
		_delay_ms(10);
	}
}

void receive_password(uint8 * password)
{
	/*
	 * Synchronize 2 MCUs
	 */
	while(UART_recieveByte() != MC1_READY){}
	UART_sendByte(MC2_READY);

	for (uint8 i=0;i<5;i++)
	{
		password[i]=UART_recieveByte();
	}
}

void Timer1_callBack(void)
{
	g_timerCount++;
}

int main(void)
{

	/*******************************************************************************
	 *                           Drivers Initializations                           *
	 *******************************************************************************/

	/* Create configuration structure for Timer_1 driver
	 * COMPARE Mode with interrupt every 3 Seconds
	 * System Clock = 8Mhz and PRESCALER F_CPU/1024.
	 * Timer frequency will be around 7.8Khz, T_timer = 128us
	 * For compare value equals to 23437 the timer will generate compare match interrupt every 3 Seconds.
	 * */
	Timer1_ConfigType Timer_1_Config = {0,23437,PRESCALER_1024,COMPARE_MODE};
	/* Set the Call back function pointer in the Timer 1 driver */
	Timer1_setCallBack(Timer1_callBack);

	/* Create configuration structure for TWI driver */
	TWI_ConfigType Twi_Config = {1,2,PRESCALE_1};
	/* Initialize the TWI/I2C Driver */
	TWI_init(&Twi_Config);

	/* Create configuration structure for UART driver */
	UART_ConfigType UART_Config = {EIGHT_BITS,DISABLED,ONE_BIT,9600};
	/* Initialize the UART driver with UART_Config */
	UART_init(&UART_Config);

	/* Initialize DC Motor */
	DcMotor_Init();
	/* Initialize the Buzzer Driver */
	Buzzer_init();

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	uint8 menu_option;
	uint8 count_wrong_password=0;

	for(;;)
	{
		switch (g_step)
		{
		/*get password case*/
		case 1:
			/*
			 * Receive First password from HMI MCU
			 */
			receive_password( g_first_received_password );
			g_step++;
			break;
		/*confirm password case*/
		case 2:
			/*
			 * Receive Second password from HMI MCU
			 */
			receive_password( g_second_received_password );

			if(passwords_is_equal(g_first_received_password,g_second_received_password))
			{
				/*
				 * Synchronize 2 MCUs
				 */
				UART_sendByte(MC2_READY);
				while(UART_recieveByte() != MC1_READY){}
				UART_sendByte(1);
				save_password_in_eeprom(g_first_received_password );
				g_step++;
			}
			else
			{
				UART_sendByte(MC2_READY);
				while(UART_recieveByte() != MC1_READY){}
				UART_sendByte(0);
				g_step=1;
			}
			break;
		/* main system option case and receive user password*/
		case 3:
			/*
			 * Synchronize 2 MCUs
			 */
			while(UART_recieveByte() != MC1_READY){}
			UART_sendByte(MC2_READY);
			/* receive menu option */
			menu_option = UART_recieveByte();
			if (menu_option == '+' || menu_option == '-')
			{
				g_step++;
			}
			else
			{
				g_step=3;
			}
			break;
		/* menu option case */
		case 4:
			/*
			 * Receive user password from HMI MCU and
			 * compare it with password in EEPROM
			 */
			receive_password( g_password );
			read_password_from_eeprom( eeprom_password );
			if(passwords_is_equal(g_password,eeprom_password))
			{
				count_wrong_password=0;
				/*
				 * Synchronize 2 MCUs
				 */
				UART_sendByte(MC2_READY);
				while(UART_recieveByte() != MC1_READY){}
				UART_sendByte(1);
				if (menu_option =='+')
				{
					g_step=5;
				}
				else if(menu_option == '-')
				{
					g_step=1;
				}
			}
			else
			{
				UART_sendByte(MC2_READY);
				while(UART_recieveByte() != MC1_READY){}
				UART_sendByte(0);
				count_wrong_password++;
				if(count_wrong_password==3)
				{
					g_step=6;
				}
			}
			break;
		/* open door case*/
		case 5:
			/* Rotate the motor CW for 15 seconds */
			DcMotor_Rotate(CW);
			/* Initialize the Timer 1 Driver */
			Timer1_init(&Timer_1_Config);
			/*
			 *  Stop the motor after 15 seconds
			 * (5 timer interrupts)(3*5=15)
			 */
			while(g_timerCount < 5);
			/* Hold motor for 3 seconds (1 timer interrupt) */
			DcMotor_Rotate(STOP);
			g_timerCount=0;
			while(g_timerCount < 1);
			/* Rotate the motor A_CW for 15 seconds */
			DcMotor_Rotate(A_CW);
			g_timerCount=0;
			while(g_timerCount < 5);
			DcMotor_Rotate(STOP);
			Timer1_deInit();
			g_timerCount=0;
			g_step=3;
			break;
		/*wrong password for 3 times case*/
		case 6:
			Buzzer_on();
			/* Initialize the Timer 1 Driver */
			Timer1_init(&Timer_1_Config);
			/* Display Error message for one minute
			 * 20 interrupts  (20*3)=60*/
			while(g_timerCount < 20);
			Buzzer_off();
			Timer1_deInit();
			g_timerCount=0;
			g_step=3;
			break;
		}
	}
}
