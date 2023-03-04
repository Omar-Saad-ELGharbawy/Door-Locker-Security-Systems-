/******************************************************************************
 *
 * File Name: HMI_main.c
 *
 * Description: main file of Human Machine Interface ECU
 *
 * Author: Omar Saad
 *
 *******************************************************************************/

#include "keypad.h"
#include "timer_1.h"
#include "lcd.h"
#include "uart.h"
#include <util/delay.h> /* For the delay functions */
#include "avr/io.h" /* To use the SREG Register */

#define MC1_READY 0x1F
#define MC2_READY 0x10
#define PRESS_TIME 150

/*******************************************************************************
 *                      Global Variables                                   *
 *******************************************************************************/

/* Arrays to store first and second passwords*/
uint8 g_first_password[5];
uint8 g_second_password[5];
uint8 g_password[5];
/* key variable to take password from keypad */
uint8 key;

uint8 g_timerCount = 0;
/* g_step for program cases */
uint8 g_step = 1;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void get_password(uint8 password[])
{
	for (uint8 i=0;i<5;i++)
	{
		/* Get the pressed key number, if any switch pressed for more than 500 ms it will considered more than one press */
		key = KEYPAD_getPressedKey();
		password[i]=key;

		if((key <= 9) && (key >= 0))
		{
			/* display the pressed keypad switch */
//			LCD_intgerToString(key);
			LCD_displayCharacter('*');
			_delay_ms(PRESS_TIME); /* Press time */
		}
		else
		{
//			LCD_displayCharacter(key); /* display the pressed keypad switch */
			LCD_displayCharacter('*');
		}
		_delay_ms(PRESS_TIME); /* Press time */
	}
}

void check_enter(void)
{
	key = KEYPAD_getPressedKey();
	if (key == '#')
	{
		_delay_ms(PRESS_TIME); /* Press time */
		g_step++;
	}
}

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

void send_password(uint8 * password)
{
	/*
	 * Synchronize 2 MCUs
	 */
	UART_sendByte(MC1_READY);
	while(UART_recieveByte() != MC2_READY){}
	for (uint8 i=0;i<5;i++)
	{
		UART_sendByte(password[i]);
	}
}

void display_password(uint8 password[])
{
	for (uint8 i=0;i<5;i++)
	{
		LCD_intgerToString(password[i]);
	}
}

void Timer1_callBack(void)
{
	g_timerCount++;
}

int main()
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

	/* Create configuration structure for UART driver */
	UART_ConfigType UART_Config = {EIGHT_BITS,DISABLED,ONE_BIT,9600};
	/* Initialize the UART driver with UART_Config */
	UART_init(&UART_Config);

	/* initialize LCD driver */
	LCD_init();

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
			LCD_clearScreen();
			LCD_displayString("Plz Enter Pass:");
			/* Move the cursor to the second row*/
			LCD_moveCursor(1,0);
			/* get first password from user and store it in g_first_password array*/
			get_password(g_first_password);
			key = KEYPAD_getPressedKey();
			/*Enter from on/c button on keypad*/
			/* check if Enter is pressed*/
			if (key == '#')
			{
				_delay_ms(PRESS_TIME); /* Press time */
				/*
				 * Send First password to CONTROL MCU
				 */
				send_password(g_first_password );
				g_step++;
			}
			break;
		/*confirm password case*/
		case 2:
			LCD_clearScreen();
			LCD_moveCursor(0,0);
			LCD_displayStringRowColumn(0,0,"Plz re-enter the");
			LCD_displayStringRowColumn(1,0,"same Pass:");
			/*Move the cursor to the second row*/
			LCD_moveCursor(1,11);
			/* get second password from user and store it in g_second_password array*/
			get_password(g_second_password);
			key = KEYPAD_getPressedKey();
			if (key == '#')
			{
				_delay_ms(PRESS_TIME); /* Press time */
				/*
				 * Send Second password to CONTROL MCU
				 */
				send_password(g_second_password );
				/*
				 * Synchronize 2 MCUs
				 */
				while(UART_recieveByte() != MC2_READY){}
				UART_sendByte(MC1_READY);
				uint8 x = UART_recieveByte();
				if(x==1)
				{
					g_step++;
				}
				else if(x==0)
				{
					g_step=1;
				}
			}
			break;
		/* take main system option and sent it to CONTROL MCU*/
		case 3 :
			LCD_clearScreen();
			LCD_moveCursor(0,0);
			LCD_displayString("+ : Open Door");
			LCD_moveCursor(1,0);
			LCD_displayString("- : Change Pass");

			menu_option = KEYPAD_getPressedKey();
			_delay_ms(PRESS_TIME); /* Press time */
			if (menu_option == '+' || menu_option == '-')
			{
				/*
				 * Synchronize 2 MCUs
				 */
				UART_sendByte(MC1_READY);
				while(UART_recieveByte() != MC2_READY){}
				UART_sendByte(menu_option);
				g_step++;
			}
			else
			{
				g_step=3;
			}
			break;
		/* Send and check password option case */
		case 4:
			LCD_clearScreen();
			LCD_moveCursor(0,0);
			LCD_displayString("Plz Enter Pass:");
			/* Move the cursor to the second row*/
			LCD_moveCursor(1,0);
			/* get first password from user and store it in g_first_password array*/
			get_password(g_password);
			key = KEYPAD_getPressedKey();
			if (key == '#')
			{
				_delay_ms(PRESS_TIME); /* Press time */
				/*
				 * Send password to CONTROL MCU to compare
				 * it with password in EEPROM
				 */
				send_password( g_password );
				/*
				 * Synchronize 2 MCUs
				 */
				while(UART_recieveByte() != MC2_READY){}
				UART_sendByte(MC1_READY);
				uint8 x = UART_recieveByte();
				_delay_ms(50);
				if(x==1)
				{
					count_wrong_password=0;
					if (menu_option =='+')
					{
						g_step=5;
					}
					else if(menu_option == '-')
					{
						g_step=1;
					}
				}
				else if(x==0)
				{
					count_wrong_password++;
					if(count_wrong_password==3)
					{
						g_step=6;
					}
				}
			}
			break;
			/* open door case*/
		case 5:

			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Door is");
			LCD_displayStringRowColumn(1,0,"Unlocking");
			/* Initialize the Timer 1 Driver */
			Timer1_init(&Timer_1_Config);
			/*
			 *  Stop the motor after 15 seconds
			 * (5 timer interrupts)(3*5=15)
			 */
			while(g_timerCount < 6);
			LCD_clearScreen();
			LCD_displayString("Door is Locking");
			g_timerCount=0;
			while(g_timerCount < 5);
			Timer1_deInit();
			g_timerCount=0;
			g_step=3;
			break;
		/*wrong password for 3 times case*/
		case 6:
			LCD_clearScreen();
			LCD_moveCursor(0,0);
			LCD_displayString("ERROR");
			/* Initialize the Timer 1 Driver */
			Timer1_init(&Timer_1_Config);
			/* Display Error message for one minute
			 * 20 interrupts  (20*3)=60*/
			while(g_timerCount < 20);
			Timer1_deInit();
			g_timerCount=0;
			g_step=3;
			break;
		default:
			LCD_clearScreen();
			LCD_displayString("System Error");
			break;
		}
	}
}
