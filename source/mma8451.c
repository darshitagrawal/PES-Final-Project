/**************************************************************************************************************************
 * File_name : - mma8451.c
 * Description: -Contains functions to initialize MMA, test MMA, read data out of it and converting the data to tilt angle.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/
#include <MKL25Z4.H>												/*Header Files*/
#include "mma8451.h"
#include "i2c.h"
#include "delay.h"
#include <math.h>
#include <stdio.h>

#define MMA_ADDR 0x3A												/*Macros*/

#define REG_XHI 0x01

#define MMA_CHECK 0X01

#define REG_WHOAMI 0x0D
#define REG_CTRL1  0x2A
#define REG_CTRL2  0x2B

#define WHOAMI 0x1A

#define COUNTS_PER_G (4096.0)
#define M_PI (3.14159265)

#define ON (1)
#define OFF (0)

#define X_REF (181)
#define Y_REF (255)
#define Z_REF (1680)

#define STANDBY_MODE 0X00
#define SELF_TEST_MODE 0X80
#define ACTIVE_MODE 0X01

static int16_t acc_X=0, acc_Y=0, acc_Z=0;									/*Global variables*/
float tilt = 0;

/*******************************************************************************************************
 * Initializes MMA
 * Function: init_mma()
 * Parameters: - None
 * Returns: - int
 *********************************************************************************************************/
int init_mma(void)
{
	i2c_write_byte(MMA_ADDR, REG_CTRL1, MMA_CHECK);					/*set active mode, 14 bit sample*/
	delay(5);

	if(i2c_read_byte(MMA_ADDR, REG_CTRL1) == MMA_CHECK)				/*Check if MMA is initialized properly*/
	{
		printf("\n\rMMA is Active.");
		return 1;
	}

	else
	{
		return 0;
	}
}

/*******************************************************************************************************
 * Test function to check the MMA 'WHO AM I' register value
 * Function: test_mma()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/
void test_mma(void)
{
	if(i2c_read_byte(MMA_ADDR, REG_WHOAMI) == WHOAMI)
	{
		printf("\n\rMMA has been successfully Initialized");
	}
}

/*******************************************************************************************************
 * Read values in all three co-ordinates from MMA.
 * Function: read_full_xyz()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/
void read_full_xyz(void)
{
	int i;
	uint8_t data[6];
	int16_t accel[3];

	i2c_start();
	i2c_read_setup(MMA_ADDR, REG_XHI);

	for(i = 0;i < 5;i++)												/*Read five bytes in repeated mode*/
	{
		data[i] = i2c_repeated_read(0);
	}

	data[i] = i2c_repeated_read(1);										/*Read last byte ending repeated mode*/

	for(i = 0;i < 3; i++)
	{
		accel[i] = (int16_t)((data[2*i] << 8) | data[2*i+1]);
	}

	acc_X = accel[0] / 4;												/*Align for 14 bits*/
	acc_Y = accel[1] / 4;
	acc_Z = accel[2] / 4;
}

/*******************************************************************************************************
 * Verify accelerometer readings.
 * Function: self_test()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/
void self_test(void)
{
	printf("\n\rNow testing the Accelerometer Readings");
	int x_sample[10], y_sample[10], z_sample[10];
	int x_sum=0, y_sum=0, z_sum=0;
	for(int i=0;i<10;i++)												/*Take 10 samples to verify our output*/
	{
		self_test_enable(ON);											/*Switch on Self test mode*/
		read_full_xyz();
		x_sample[i]=acc_X;
		y_sample[i]=acc_Y;
		z_sample[i]=acc_Z;

		self_test_enable(OFF);											/*Switch off Self test mode*/
		read_full_xyz();
		x_sample[i]-=acc_X;
		y_sample[i]-=acc_Y;
		z_sample[i]-=acc_Z;

		x_sum+=x_sample[i];
		y_sum+=y_sample[i];
		z_sum+=z_sample[i];

	}
	if((x_sum/10 > X_REF) && (y_sum/10 > Y_REF) && (z_sum/10 > Z_REF))	/*Check the average with the reference values*/
	{
		printf("\n\rAccelerometer readings has been successfully verified.");
	}
	else
	{
		printf("\n\rAccelerometer readings has been failed to verify.");
	}
}

/*******************************************************************************************************
 * Enable/ disable self-test of MMA.
 * Function: self_test_enable()
 * Parameters: - state
 * state: Specify whether to enable self_test mode.
 * Returns: - None
 *********************************************************************************************************/
void self_test_enable(int state)
{
	if(state)
	{
		i2c_write_byte(MMA_ADDR, REG_CTRL1, STANDBY_MODE);						/*Standby mode*/
		delay(5);
		i2c_write_byte(MMA_ADDR, REG_CTRL2, SELF_TEST_MODE);					/*Self-test mode*/
		delay(5);
		i2c_write_byte(MMA_ADDR, REG_CTRL1, ACTIVE_MODE);						/*Active mode*/
		delay(5);
	}
	else if(!state)
	{
		i2c_write_byte(MMA_ADDR, REG_CTRL1, STANDBY_MODE);						/*Standby mode*/
		delay(5);
		i2c_write_byte(MMA_ADDR, REG_CTRL2, STANDBY_MODE);						/*Coming out of Self-test mode*/
		delay(5);
		i2c_write_byte(MMA_ADDR, REG_CTRL1, ACTIVE_MODE);						/*Active mode*/
		delay(5);
	}
}

/*******************************************************************************************************
 * Convert accelerometer readings to tilt angle.
 * Function: convert_xyz_to_roll_pitch()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/
void convert_xyz_to_roll_pitch(void)
{
	float ax = acc_X/COUNTS_PER_G, ay = acc_Y/COUNTS_PER_G, az = acc_Z/COUNTS_PER_G;
	float tilt_value;
	tilt_value = (az / sqrt(ax*ax + ay*ay + az*az));
	tilt = acos(tilt_value) * 180/M_PI;
}
