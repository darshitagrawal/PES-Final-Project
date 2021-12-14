/**************************************************************************************************************************
 * File_name : - uart.c
 * Description: - Initializes UART0 and contains IRQHandler for UART0. Contains terminal function that
 * prints message at the start of the terminal.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 *3) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 ***************************************************************************************************************************/
#include "MKL25Z4.h"									/*Header Files*/
#include "uart.h"
#include "queue.h"
#include <stdio.h>
#include "terminal.h"

#define BAUD_RATE 38400									/*Macros*/
#define UART_OVERSAMPLE_RATE 16
#define SYS_CLOCK (24e6)
#define PARITY 0
#define STOP_BITS 1
#define DATA 0

/*******************************************************************************************************
 * Initializes UART0 for 38400 baud rate, 2 stop bits, no parity bit and for interrupts.
 * Function: UART0_init()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/

void UART0_init(void)
{
	__disable_irq();									/*global disable IRQs*/

	uint16_t sbr;
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;					/*Enable clock for UART0*/
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;					/*Enable clock for PORT A*/
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;	/*turn off UART0 while changing configurations*/

	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);				/*Use FLL output for UART Baud rate generator*/
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);/*Set pins to UART0 Tx and Rx*/

	sbr = (uint16_t)((SYS_CLOCK)/(BAUD_RATE * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;					/*Setting 38400 baud-rate*/
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE - 1);/*Setting over-sampling ratio*/
														/*Disable interrupts for RX active edge and LIN break detect, select two stop bits*/
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOP_BITS) | UART0_BDH_LBKDIE(0);
														/*Don't enable loopback mode, use 8 data bit mode, don't use parity*/
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(DATA) | UART0_C1_PE(PARITY);
														/*Enable Receiver and Transmitter and receive interrupt*/
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(1) | UART0_C3_NEIE(1) | UART0_C3_FEIE(1) | UART0_C3_PEIE(1);
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);
	UART0->C2 = UART0_C2_TE(1) | UART0_C2_RE(1) | UART0_C2_RIE(1);

	Queue_Init(&transmit_t);							/*Initialize transmit buffer*/
	Queue_Init(&receive_t);								/*Initialize receive buffer*/

	NVIC_SetPriority(UART0_IRQn,3);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);							/*Enable interrupt in the interrupt vector table*/

	__enable_irq();										/*global enable IRQs*/
}

/*******************************************************************************************************
 * IRQHandler for UART0. It is executed whenever transmit or receive interrupt is generated.
 * Function: UART0_IRQHandler
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/
void UART0_IRQHandler(void)
{
	char value, dequeue_data;
	if (UART0->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK))
	{													/*Clear the error flags*/
		UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK | UART0_S1_FE_MASK | UART0_S1_PF_MASK;
		value = UART0->D;								/*Clear RDRF*/
	}
	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		value = UART0->D;								/*Received a character*/
		cbfifo_enqueue(&receive_t, &value, 1);			/*Enqueue the received byte*/
	}
	 	 	 	 	 	 	 	 	 	 	 	 	 	/*transmitter interrupt enabled*/
	if ((UART0->C2 & UART0_C2_TIE_MASK) && (UART0->S1 & UART0_S1_TDRE_MASK))
	{
		if(!Queue_Empty(&transmit_t))
		{												/*Dequeue from transmit buffer*/
			cbfifo_dequeue(&transmit_t, &dequeue_data, 1);
			UART0->D = dequeue_data;					/*Send a byte over UART*/
		}
		else
		{
			UART0->C2 &= ~UART0_C2_TIE_MASK;			/*queue is empty so disable transmitter interrupt*/
		}
	}
}

/*******************************************************************************************************
 * Function to retarget Redlib's printf().
 * Function: __sys_write()
 * Parameters: - int handler, char* buf, int count
 * Returns: - int
 *********************************************************************************************************/
int __sys_write(int handle, char* buf, int count)
{
	if(buf == NULL)										/*return -1 if data pointer is NULL*/
	{
		return -1;
	}
	while(Queue_Full(&transmit_t))						/*Wait for transmit buffer to have space*/
	{
		;
	}

	if(cbfifo_enqueue(&transmit_t, buf, count) != count)/*return -1 if the number of bytes enqueued doesn't match the return value of cbfifo_enqueue()*/
	{
		return -1;
	}

	if(!(UART0->C2 & UART0_C2_TIE_MASK))				/*Enable transmit interrupt*/
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}

	return 0;											/*return 0 on success*/
}

/*******************************************************************************************************
 * Function to retarget Redlib's scanf().
 * Function: __sys_readc()
 * Parameters: - None
 * Returns: - int
 *********************************************************************************************************/
int __sys_readc(void)
{
	char ch;
	if (cbfifo_enqueue(&receive_t, &ch, 1) != 1)		/*return -1 if the number of bytes enqueued doesn't match the return value of cbfifo_enqueue()*/
	{
		return -1;
	}
	return ch;
}



