/**************************************************************************************************************************
 * File_name : - LEDs.h
 * Description: -Contains prototypes of functions to initialize TPM module for PWM based LED lights.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/

#ifndef LEDS_H
#define LEDS_H

/*******************************************************************************************************
 * Initializes TPM modules for generating PWM.
 * Function: Init_RGB_LEDs()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/

void Init_RGB_LEDs(void);

/*******************************************************************************************************
 * Sets the PWM value for tricolor LEDs.
 * Function: Control_RGB_LEDs()
 * Parameters: - int red_on, int green_on, int blue_on
 * red_on: PWM value for RED LED
 * green_on: PWM value for GREEN LED
 * blue_on: PWM value for BLUE LED
 * Returns: - None
 *********************************************************************************************************/

void Control_RGB_LEDs(int red_on,int green_on,int blue_on);

/*******************************************************************************************************
 * Tests the RGB tricolor LED by turning on RED, GREEN, BLUE LED successively for approx. one second.
 * Function: RGB_test()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/

void RGB_test(void);

#endif
