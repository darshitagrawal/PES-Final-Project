/**************************************************************************************************************************
 * File_name : - uart.h
 * Description: - Contains prototype of functions to initialize UART0 and contains IRQHandler for UART0. Contains terminal function that
 * prints message at the start of the terminal.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 *3) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 ***************************************************************************************************************************/

#ifndef UART_H_
#define UART_H_

/*******************************************************************************************************
 * Initializes UART0 for 38400 baud rate, 2 stop bits, no parity bit and for interrupts.
 * Function: UART0_init()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/

void UART0_init(void);

#endif /* UART_H_ */
