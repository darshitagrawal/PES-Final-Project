/**************************************************************************************************************************
 * File_name : - i2c.c
 * Description: -Contains functions for I2C initialization, read, write and generating start, stop and repeated start bits.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/

#include <stdio.h>															/*Header Files*/
#include <stdint.h>
#include "MKL25Z4.h"
#include "i2c.h"

#define I2C_M_START				I2C0->C1 |= I2C_C1_MST_MASK					/*Macros*/
#define I2C_M_STOP				I2C0->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART			I2C0->C1 |= I2C_C1_RSTA_MASK

#define I2C_TRAN				I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC					I2C0->C1 &= ~I2C_C1_TX_MASK

#define I2C_WAIT				while((I2C0->S & I2C_S_IICIF_MASK) == 0) {} \
								I2C0->S |= I2C_S_IICIF_MASK;

#define NACK					I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK 					I2C0->C1 &= ~I2C_C1_TXAK_MASK

static int lock_detect = 0;														/*Global variable*/

/*******************************************************************************************************
 * Initializes I2C0 peripheral connected to the accelerometer.
 * Function: i2c_init()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/
void i2c_init(void)
{
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;										/*Clock I2C peripheral and Port E*/
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	PORTE->PCR[24] |= PORT_PCR_MUX(5);										/*set pins to I2C function*/
	PORTE->PCR[25] |= PORT_PCR_MUX(5);

	I2C0->F = I2C_F_ICR(0X10) | I2C_F_MULT(0);								/*Set the baud-rate*/

	I2C0->C1 |= (I2C_C1_IICEN_MASK);										/*Enable I2C and set to master mode*/

	I2C0->C2 |= (I2C_C2_HDRS_MASK);											/*Select high drive mode*/
}

/*******************************************************************************************************
 * Generates the start bit to initiate read/write operation..
 * Function: i2c_start()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/
void i2c_start(void)
{
	I2C_TRAN;																/*Set to transmit mode */
	I2C_M_START;															/*Send start*/
}

/*******************************************************************************************************
 * Send device and register addresses and setup the read operation
 * Function: i2c_read_setup()
 * Parameters: - uint8_t dev, uint8_t address
 * dev: device address
 * address: register address
 * Returns: - None
 *********************************************************************************************************/

void i2c_read_setup(uint8_t dev, uint8_t address)
{
	I2C0->D = dev;			  												/*Send device address	*/
	I2C_WAIT																/*Wait for completion */

	I2C0->D =  address;														/*Send read address	*/
	I2C_WAIT																/*Wait for completion */

	I2C_M_RSTART;				   											/*Repeated start */
	I2C0->D = (dev|0x1);	 												/*Send device address (read)	*/
	I2C_WAIT							 									/*Wait for completion */

	I2C_REC;						   										/*Set to receive mode */
}

/*******************************************************************************************************
 * Read a byte and ack/nack as appropriate.
 * Function: i2c_repeated_read()
 * Parameters: - uint8_t isLastRead
 * isLastRead: Indicates if the last byte of data is to be read.
 * Returns: - uint8_t data
 * data: Data that has been read from the address.
 *********************************************************************************************************/
uint8_t i2c_repeated_read(uint8_t isLastRead)
{
	uint8_t data;

	lock_detect = 0;

	if(isLastRead)
	{
		NACK;																/*Set NACK after read	*/
	}
	else
	{
		ACK;																/*ACK after read	*/
	}

	data = I2C0->D;															/*Dummy read	*/
	I2C_WAIT																/*Wait for completion */

	if(isLastRead)
	{
		I2C_M_STOP;															/*Send stop	*/
	}

	data = I2C0->D;															/*Read data	*/

	return  data;
}

/*******************************************************************************************************
 * Using 7 bit addressing, writes a byte of data to the device address.
 * Function: i2c_write_byte()
 * Parameters: - uint8_t dev, uint8_t reg, uint8_t data
 * dev: Device address
 * reg: Register address
 * data: Data to be sent
 * Returns: - None
 *********************************************************************************************************/

void i2c_write_byte(uint8_t dev, uint8_t reg, uint8_t data)
{
	I2C_TRAN;																/*Set to transmit mode*/
	I2C_M_START;															/*Send start*/
	I2C0->D = dev;															/*Send device address (write)*/
	I2C_WAIT																/*Wait for ack*/

	I2C0->D = reg;															/*Send register address*/
	I2C_WAIT

	I2C0->D = data;															/*Send data*/
	I2C_WAIT
	I2C_M_STOP;																/*Send stop*/
}

/*******************************************************************************************************
 * Using 7 bit addressing, reads a byte from device address.
 * Function: i2c_read_byte()
 * Parameters: - uint8_t dev, uint8_t reg
 * dev: Device address
 * reg: Register address
 * Returns: uint8_t data
 * data: Data that has been read from the address
 *********************************************************************************************************/

uint8_t i2c_read_byte(uint8_t dev, uint8_t reg)
{
	uint8_t data;

	I2C_TRAN;																/*Set to transmit mode*/
	I2C_M_START;															/*Send start*/
	I2C0->D = dev;															/*Send device address (write)*/
	I2C_WAIT																/*Wait for completion*/

	I2C0->D = reg;															/*Send register address*/
	I2C_WAIT																/*Wait for completion*/

	I2C_M_RSTART;															/*Repeated start*/
	I2C0->D = (dev | 0X1);													/*Send device address (read)*/
	I2C_WAIT																/*Wait for completion*/

	I2C_REC;																/*Set to receive mode*/
	NACK;																	/*Set NACK after read*/

	data = I2C0->D;															/*Dummy read*/
	I2C_WAIT																/*Wait for completion*/

	I2C_M_STOP;																/*Send stop*/
	data = I2C0->D;															/*Read data*/

	return data;
}


