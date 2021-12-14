/**************************************************************************************************************************
 * File_name : - terminal.h
 * Description: - Contains prototypes for functions processing commands given in the terminal.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/

#ifndef TERMINAL_H_
#define TERMINAL_H_

/*******************************************************************************************************
 * Function to take commands from the user until carriage return or line feed is detected.
 * Function: terminal_input()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/

void terminal_input(void);

/*******************************************************************************************************
 * Function to process commands given by the user.
 * Function: lexical_analysis()
 * Parameters: - char* input
 * *input: Input command given by the user.
 * Returns: - None
 *********************************************************************************************************/

void lexical_analysis(char *input);

/*******************************************************************************************************
 * Function to display message at the start of the Terminal.
 * Function: Terminal()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/

void Terminal(void);

#endif /* TERMINAL_H_ */
