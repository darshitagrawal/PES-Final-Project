/**************************************************************************************************************************
 * File_name : - switch.c
 * Description: -Contains functions to initialize GPIO interrupt at Port D and its IRQHandler.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/
#include <MKL25Z4.H>												/*Header Files*/
#include <stdio.h>
#include <math.h>
#include "switch.h"
#include "mma8451.h"

#define MASK(x) (1UL << (x))										/*Macros*/
#define SW1_POS (4)
#define SW2_POS (5)

#define RESET_STATUS_FLAGS (0XFFFFFFFF)
#define INTERRUPT_DETECTION_FLAGS (0X30)
#define PORTD_PIN4_INTERRUPT (0X10)
#define PORTD_PIN5_INTERRUPT (0X20)

float calibrated_angle = 0;											/*Global variables*/
unsigned int break_flag = 1, one_time_flag = 0;

/*******************************************************************************************************
 * Initialize Port D pin 4 and pin 5 for GPIO interrupt.
 * Function: init_switch()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/
void init_switch(void)
{
	/*Disable global IRQs*/
	__disable_irq();
	/*Enable clock to Port D*/
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	/*Select GPIO and enable pull-up resistors and interrupts on falling edge*/
	PORTD->PCR[SW1_POS] = PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(10);
	/*Make pin input*/
	PTD->PDDR &= ~MASK(SW1_POS);

	/*Select GPIO and enable pull-up resistors and interrupts on falling edge*/
	PORTD->PCR[SW2_POS] = PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(10);
	/*Make pin input*/
	PTD->PDDR &= ~MASK(SW2_POS);

	/*Configure NVIC*/
	NVIC_SetPriority(PORTD_IRQn,2);
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn);
	/*Enable global IRQs*/
	__enable_irq();
}
/*******************************************************************************************************
 * Interrupt Handler for GPIO interrupt at Port D.
 * Function: PORTD_IRQHandler()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/
void PORTD_IRQHandler(void)
{
	while(PORTD->ISFR & INTERRUPT_DETECTION_FLAGS)
	{
		if(PORTD->ISFR & PORTD_PIN4_INTERRUPT)
		{
			calibrated_angle = fabs(tilt) - calibrated_angle;
			one_time_flag = 1;
		}

		if(PORTD->ISFR & PORTD_PIN5_INTERRUPT)
		{
			break_flag = 0;
		}
		/*Clear interrupt flag*/
		PORTD->ISFR = RESET_STATUS_FLAGS;
	}
}
