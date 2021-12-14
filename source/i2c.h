/**************************************************************************************************************************
 * File_name : - i2c.h
 * Description: -Contains prototypes of functions for I2C initialization, read, write and generating start, stop and repeated start bits.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/

#ifndef I2C_H_
#define I2C_H_

/*******************************************************************************************************
 * Initializes I2C0 peripheral connected to the accelerometer.
 * Function: i2c_init()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/

void i2c_init(void);

/*******************************************************************************************************
 * Generates the start bit to initiate read/write operation..
 * Function: i2c_start()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/

void i2c_start(void);

/*******************************************************************************************************
 * Send device and register addresses and setup the read operation
 * Function: i2c_read_setup()
 * Parameters: - uint8_t dev, uint8_t address
 * dev: device address
 * address: register address
 * Returns: - None
 *********************************************************************************************************/

void i2c_read_setup(uint8_t dev, uint8_t address);

/*******************************************************************************************************
 * Read a byte and ack/nack as appropriate.
 * Function: i2c_repeated_read()
 * Parameters: - uint8_t isLastRead
 * isLastRead: Indicates if the last byte of data is to be read.
 * Returns: - uint8_t data
 * data: Data that has been read from the address.
 *********************************************************************************************************/

uint8_t i2c_repeated_read(uint8_t isLastRead);

/*******************************************************************************************************
 * Using 7 bit addressing, writes a byte of data to the device address.
 * Function: i2c_write_byte()
 * Parameters: - uint8_t dev, uint8_t reg, uint8_t data
 * dev: Device address
 * reg: Register address
 * data: Data to be sent
 * Returns: - None
 *********************************************************************************************************/

void i2c_write_byte(uint8_t dev, uint8_t reg, uint8_t data);

/*******************************************************************************************************
 * Using 7 bit addressing, reads a byte from device address.
 * Function: i2c_read_byte()
 * Parameters: - uint8_t dev, uint8_t reg
 * dev: Device address
 * reg: Register address
 * Returns: uint8_t data
 * data: Data that has been read from the address
 *********************************************************************************************************/

uint8_t i2c_read_byte(uint8_t dev, uint8_t reg);

#endif /* I2C_H_ */
