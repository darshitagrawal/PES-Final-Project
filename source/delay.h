/**************************************************************************************************************************
 * File_name : - delay.h
 * Description: - Contains function prototype to generate delay
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/

#ifndef DELAY_H
#define DELAY_H

/*******************************************************************************************************
 * Generates delay between two commands.
 * Function: delay()
 * Parameters: - uint32_t time
 * time: Number of ticks up to which delay is to be provided
 * Returns: - None
 *********************************************************************************************************/

void delay(uint32_t time);

/*******************************************************************************************************
 * Generates delay of one second.
 * Function: one_sec_delay()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/

void one_sec_delay(void);

#endif
