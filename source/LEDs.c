/**************************************************************************************************************************
 * File_name : - LEDs.c
 * Description: -Contains functions to initialize TPM module for PWM based LED lights.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/
#include <MKL25Z4.H>													/*Header Files*/
#include <stdio.h>
#include "LEDs.h"
#include "delay.h"

#define RED_LED_POS (18)												/*Macros*/
#define GREEN_LED_POS (19)
#define BLUE_LED_POS (1)
#define PWM (48000)
#define COLOR_STEP (187.5)

#define MASK(x) (1UL << (x))

/*******************************************************************************************************
 * Initializes TPM modules and RGB LEDs for generating PWM.
 * Function: Init_RGB_LEDs()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/
void Init_RGB_LEDs(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;			/*Enable clock to Port B and Port D*/

	PORTB->PCR[RED_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(3);							/*Selecting pin alternating function 4*/
	PORTB->PCR[GREEN_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_POS] |= PORT_PCR_MUX(3);						/*Selecting pin alternating function 4*/
	PORTD->PCR[BLUE_LED_POS] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_POS] |= PORT_PCR_MUX(4);						/*Selecting pin alternating function 5*/

	PTB->PDDR |= MASK(RED_LED_POS) | MASK(GREEN_LED_POS);				/*Selecting ports as output*/
	PTD->PDDR |= MASK(BLUE_LED_POS);

	SIM->SOPT2 |=(SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);		/*Set clock source*/

	SIM->SCGC6 |=SIM_SCGC6_TPM0_MASK;									/*Enabling clock to TPM0*/

	TPM0->MOD = PWM - 1;												/*Load TPM0 Mod register*/
	TPM0->SC = TPM_SC_PS(1);											/*Set prescaler as 2*/
	TPM0->CONF |= TPM_CONF_DBGMODE(3);									/*Continue operation in Debug mode*/
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;	/*PWM mode, toggle output on match*/
	TPM0->CONTROLS[1].CnV = 0;											/*Load Channel value register*/
	TPM0->SC |= TPM_SC_CMOD(1);											/*Start TPM*/

	SIM->SCGC6 |=SIM_SCGC6_TPM2_MASK;									/*Enabling clock to TPM2*/
	TPM2->MOD = PWM - 1;												/*Load TPM0 Mod register*/
	TPM2->SC = TPM_SC_PS(1);											/*Set prescaler as 2*/
	TPM2->CONF |= TPM_CONF_DBGMODE(3);									/*Continue operation in Debug mode*/
	TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;	/*PWM mode, toggle output on match*/
	TPM2->CONTROLS[1].CnV = 0;											/*Load Channel value register*/
	TPM2->SC |= TPM_SC_CMOD(1);											/*Start TPM*/

	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;	/*PWM mode, toggle output on match*/
	TPM2->CONTROLS[0].CnV = 0;											/*Load Channel value register*/

}

/*******************************************************************************************************
 * Sets the PWM value for tricolor LEDs.
 * Function: Control_RGB_LEDs()
 * Parameters: - int red_on, int green_on, int blue_on
 * red_on: PWM value for RED LED
 * green_on: PWM value for GREEN LED
 * blue_on: PWM value for BLUE LED
 * Returns: - None
 *********************************************************************************************************/

void Control_RGB_LEDs(int red_on,int green_on,int blue_on)
{
	TPM2->CONTROLS[0].CnV = COLOR_STEP*red_on;							/*Setting the PWM value for RED LED*/
	TPM2->CONTROLS[1].CnV = COLOR_STEP*green_on;						/*Setting the PWM value for GREEN LED*/
	TPM0->CONTROLS[1].CnV = COLOR_STEP*blue_on;							/*Setting the PWM value for BLUE LED*/
}

/*******************************************************************************************************
 * Tests the RGB tricolor LED by turning on RED, GREEN, BLUE LED successively for approx. one second.
 * Function: RGB_test()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/
void RGB_test(void)
{
	printf("\n\rTesting Tricolor LED");
	one_sec_delay();
	Control_RGB_LEDs(255,0,0);											/*Turn on RED LED*/
	one_sec_delay();
	Control_RGB_LEDs(0,255,0);											/*Turn on GREEN LED*/
	one_sec_delay();
	Control_RGB_LEDs(0,0,255);											/*Turn on BLUE LED*/
	one_sec_delay();
	Control_RGB_LEDs(0,0,0);											/*No LED*/
}
