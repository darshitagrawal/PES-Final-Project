/**************************************************************************************************************************
 * File_name : - delay.c
 * Description: - Contains function to generate delay
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/

#include <MKL25Z4.H>								/*Header Files*/

/*******************************************************************************************************
 * Generates delay between two commands.
 * Function: delay()
 * Parameters: - uint32_t time
 * time: Number of ticks up to which delay is to be provided
 * Returns: - None
 *********************************************************************************************************/
void delay(uint32_t time)
{
	volatile uint32_t tick;
	for(tick = 0; tick < time * 10000; tick++)
			;
}

/*******************************************************************************************************
 * Generates delay of one second.
 * Function: one_sec_delay()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/
void one_sec_delay(void)
{
	for(int i = 0;i < 1000;i++)
	{
		for(int j = 0;j < 1000;j++);
	}
}
