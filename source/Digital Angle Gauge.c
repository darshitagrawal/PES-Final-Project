/**************************************************************************************************************************
 * File_name : - main.c
 * Description: -Contains functions to initialize Board hardware, system clock, TPM modules, UART0, I2C0, GPIO interrupt and
 * their test fucntions.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/
#include <MKL25Z4.H>
#include <stdio.h>
#include <math.h>
#include <board.h>
#include "LEDs.h"
#include "i2c.h"
#include "mma8451.h"
#include "delay.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "uart.h"
#include "queue.h"
#include "sysclock.h"
#include "switch.h"
#include "queue_test.h"
#include "terminal.h"

/*******************************************************************************************************
 * Contains functions to initialize Board hardware, system clock, TPM modules, UART0, I2C0, GPIO interrupt and
 * their test fucntions.
 * Function: Init_RGB_LEDs()
 * Parameters: - None
 * Returns: - int
 *********************************************************************************************************/
int main (void) {

	BOARD_InitBootPins();								/*Initialize board hardware*/
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	BOARD_InitDebugConsole();
#endif

	sysclock_init();									/*Initialize System Clock*/
	Init_RGB_LEDs();									/*Initialize TPM modules and RGB LEDs*/
	UART0_init();										/*Initialize UART0*/
	test_cbfifo();										/*CBFIFO test function*/
	RGB_test();											/*Test tricolor LED*/
	i2c_init();											/*Initialize I2C*/
	init_switch();										/*Initialize GPIO interrupt*/
	if (!init_mma())
	{
		Control_RGB_LEDs(255, 0, 255);					/*Glow Violet color if MMA initialization fails*/
		while (1)
			;
	}

	test_mma();											/*Test function to test MMA and I2C*/
	self_test();										/*Test function to test accelerometer output validity*/
	while (1)
	{
		Terminal();
	}
}
