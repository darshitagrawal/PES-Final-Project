/**************************************************************************************************************************
 * File_name : - mma8451.h
 * Description: -Contains prototypes of functions to initialize MMA, test MMA, read data out of it and converting the data to tilt angle.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/

#ifndef MMA8451_H
#define MMA8451_H

/*******************************************************************************************************
 * Initializes MMA
 * Function: init_mma()
 * Parameters: - None
 * Returns: - int
 *********************************************************************************************************/

int init_mma(void);

/*******************************************************************************************************
 * Read values in all three co-ordinates from MMA.
 * Function: read_full_xyz()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/

void read_full_xyz(void);

/*******************************************************************************************************
 * Convert accelerometer readings to tilt angle.
 * Function: convert_xyz_to_roll_pitch()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/

void convert_xyz_to_roll_pitch(void);

/*******************************************************************************************************
 * Test function to check the MMA 'WHO AM I' register value
 * Function: test_mma()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/

void test_mma(void);

/*******************************************************************************************************
 * Verify accelerometer readings.
 * Function: self_test()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/

void self_test(void);

/*******************************************************************************************************
 * Enable/ disable self-test of MMA.
 * Function: self_test_enable()
 * Parameters: - state
 * state: Specify whether to enable self_test mode.
 * Returns: - None
 *********************************************************************************************************/

void self_test_enable(int state);

extern float tilt;							/*Extern global variables*/

#endif
