/**************************************************************************************************************************
 * File_name : - switch.h
 * Description: -Contains prototype of function to initialize GPIO interrupt at Port D.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/

#ifndef SWITCH_H_
#define SWITCH_H_

/*******************************************************************************************************
 * Initialize Port D pin 4 and pin 5 for GPIO interrupt.
 * Function: init_switch()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/

void init_switch(void);

#endif /* SWITCH_H_ */
