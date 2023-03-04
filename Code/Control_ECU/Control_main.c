///******************************************************************************
// *
// * File Name: Control_main.c
// *
// * Description: main file of Control ECU
// *
// * Author: Omar Saad
// *
// *******************************************************************************/
//#include "timer_1.h"
//#include "buzzer.h"
////#include "keypad.h"
//#include "lcd.h"
//#include "twi.h"
//#include "DcMotor.h"
//#include "external_eeprom.h"
//#include "uart.h"
//#include "avr/io.h" /* To use the IO Ports Registers */
//#include <util/delay.h> /* For the delay functions */
//
//#define MC1_READY 0x1F
//#define MC2_READY 0x10
//
///*******************************************************************************
// *                      Global Variables                                   *
// *******************************************************************************/
//
///* Arrays to store first and second passwords*/
//uint8 g_first_password[5];
//uint8 eeprom_password[5];
//uint8 key;
//uint8 g_my_password[5]={1,2,3,4,5};
//
//uint8 g_timerCount = 0;
//
//void get_password(uint8 password[])
//{
//	for (uint8 i=0;i<5;i++)
//	{
//		/* Get the pressed key number, if any switch pressed for more than 500 ms it will considered more than one press */
//		key = KEYPAD_getPressedKey();
//		password[i]=key;
//
//		if((key <= 9) && (key >= 0))
//		{
//			/* display the pressed keypad switch */
//			LCD_intgerToString(key);
//			//			LCD_displayCharacter('*');
//			_delay_ms(500); /* Press time */
//		}
//		else
//		{
//			LCD_displayCharacter(key); /* display the pressed keypad switch */
//		}
//		_delay_ms(500); /* Press time */
//	}
//	//		password[6]='#';
//	//		password[7]='\0';
//}
//
//void display_password(uint8 password[])
//{
//	for (uint8 i;i<5;i++)
//	{
//		LCD_intgerToString(password[i]);
//	}
//}
//
//boolean passwords_is_equal(uint8 password_1[],uint8 password_2[])
//{
//	boolean flag;
//	for (uint8 i=0;i<5;i++)
//	{
//		if (password_1[i]==password_2[i])
//		{
//			flag = TRUE;
//		}
//		else
//		{
//			flag = FALSE;
//			break;
//		}
//	}
//	return flag;
//}
//
//void save_password_in_eeprom(uint8 * password)
//{
//	for (uint8 i=0;i<5;i++)
//	{
//		EEPROM_writeByte(0x0311+i, password[i]); /* Write 0x0F in the external EEPROM */
//		_delay_ms(10);
//	}
//}
//
//void read_password_from_eeprom(uint8 * password)
//{
//	for (uint8 i=0;i<5;i++)
//	{
//		EEPROM_readByte(0x0311+i, &password[i]);  /* Read 0x0F from the external EEPROM */
//		_delay_ms(10);
//	}
//}
//
//void Timer1_callBack(void)
//{
//	g_timerCount++;
//}
//
//int main(void)
//{
//
//	/* Create configuration structure for Timer_1 driver */
//	Timer1_ConfigType Timer_1_Config = {0,23437,PRESCALER_1024,COMPARE_MODE};
//	/* Set the Call back function pointer in the Timer 1 driver */
//	Timer1_setCallBack(Timer1_callBack);
//
//	/* Create configuration structure for TWI driver */
//	TWI_ConfigType Twi_Config = {1,2,PRESCALE_1};
//	/* Initialize the TWI/I2C Driver */
//	TWI_init(&Twi_Config);
//
//	/* Initialize DC Motor */
//	DcMotor_Init();
//	/* Initialize the LCD Driver */
////	LCD_init();
//	/* Initialize the Buzzer Driver */
//	Buzzer_init();
////	uint8 val = 0;
//	//	/* Create configuration structure for UART driver */
//	//	UART_ConfigType UART_Config = {EIGHT_BITS,DISABLED,ONE_BIT,9600};
//	//	/* Initialize the UART driver with UART_Config */
//	//	UART_init(&UART_Config);
//
//	save_password_in_eeprom(g_my_password);
//
//	LCD_moveCursor(0,0);
//	LCD_displayString("Password is");
//
//	/* Enable Global Interrupt I-Bit */
//	SREG |= (1<<7);
//
//	for(;;)
//	{
//
//		//		DcMotor_Rotate(A_CW);
//		LCD_clearScreen();
//		LCD_displayString("Plz Enter Pass:");
//		LCD_moveCursor(1,0); /* Move the cursor to the second row*/
//		/* get first password from user and store it in g_first_password array*/
//		get_password(g_first_password);
//		key = KEYPAD_getPressedKey();
//
//		if (key == '#')
//		{
//			_delay_ms(500); /* Press time */
//			LCD_clearScreen();
//			display_password(g_first_password);
//			_delay_ms(500);
//			read_password_from_eeprom(eeprom_password);
//			LCD_clearScreen();
//			if(passwords_is_equal(g_first_password,eeprom_password))
//			{
//				LCD_displayString("Equal");
//				DcMotor_Rotate(CW);
//				Buzzer_on();
//				/* Initialize the Timer 1 Driver */
//				Timer1_init(&Timer_1_Config);
//				/*
//				 * Stop the motor after 3 seconds
//				 */
//				while(g_timerCount < 1);
//
//				DcMotor_Rotate(STOP);
//				LCD_displayString("Motor Stopped");
//				Buzzer_off();
//				_delay_ms(5000);
////				Timer1_deInit();
//
//
//			}
//			//			else
//			//			{
//			//				LCD_displayString("Not Equal");
//			//				DcMotor_Rotate(STOP);
//			//			}
//
//			//			display_password(eeprom_password);
//			//			_delay_ms(5000);
//
//		}
//
//
//		//		LCD_moveCursor(1,0);
//		//		/*
//		//		 * Synchronize 2 MCUs
//		//		 */
//		//		while(UART_recieveByte() != MC1_READY){}
//		//		UART_sendByte(MC2_READY);
//		//		/*
//		//		 * Receive First password from HMI MCU
//		//		 */
//		//		for (uint8 i;i<5;i++)
//		//		{
//		//			g_first_password[i]=UART_recieveByte();
//		//		}
//		//
//		//		LCD_moveCursor(1,0);
//		//		for (uint8 i;i<5;i++)
//		//		{
//		//			LCD_intgerToString(g_first_password[i]);
//		//		}
//		//		_delay_ms(5000);
//	}
//}
