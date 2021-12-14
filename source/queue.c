/**************************************************************************************************************************
 * File_name : - queue.c
 * Description: - Initializes cbfifo and contains functions for enqueuing and dequeuing data into cbfifo.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/
#include "queue.h"							/*Header Files*/

Queue_T transmit_t, receive_t;				/*Global structure variables*/

/*******************************************************************************************************
 * Initializes cbfifo at the beginning of enqueuing or dequeuing data.
 * Function: Queue_Init()
 * Parameters: - Queue_T *q
 * *q: Pointer to the structure to be intialized.
 * Returns: - None
 *********************************************************************************************************/
void Queue_Init(Queue_T * q)
{
	unsigned int i;
	for (i=0; i<Q_MAX_SIZE; i++)
	{
		q->Data[i] = '_';
		q->Front = 0;
		q->Rear = 0;
		q->Size = 0;
		q->full = false;
	}
}

/*******************************************************************************************************
 * Function to check whether the cbfifo is empty or not and returns relevant value as per the status.
 * Function: Queue_Empty
 * Parameters: - Queue_T *q
 * *q: Pointer to the structure
 * Returns: - bool
 *********************************************************************************************************/
bool Queue_Empty(Queue_T * q)
{
	return ((q->Rear == q->Front) && (!q->full));
}

/*******************************************************************************************************
 * Function to check whether the cbfifo is full or not and returns relevant value as per the status.
 * Function: Queue_Full
 * Parameters: - Queue_T *q
 * *q: Pointer to the structure
 * Returns: - bool
 *********************************************************************************************************/
bool Queue_Full(Queue_T * q)
{
	return (cbfifo_length(q) == Q_MAX_SIZE);
}

/*******************************************************************************************************
 * Function to return the number of elements currently in the cbfifo.
 * Function: Queue_Size
 * Parameters: - Queue_T *q
 * *q: Pointer to the structure
 * Returns: - int q->Size
 * q->Size: Size of the structure
 *********************************************************************************************************/
int Queue_Size(Queue_T * q)
{
	return q->Size;
}

/*******************************************************************************************************
 * Function to return the number of elements currently in the cbfifo.
 * Function: cbfifo_length()
 * Parameters: - Queue_T *q
 * *q: Pointer to the structure
 * Returns: - size_t q->Size
 * q->Size: Size of the structure
 *********************************************************************************************************/
size_t cbfifo_length(Queue_T *q)
{
    return q->Size;
}

/*******************************************************************************************************
 * Function to return the total capacity of the cbfifo.
 * Function: cbfifo_capacity()
 * Parameters: - Queue_T *q
 * *q: Pointer to the structure
 * Returns: - size_t Q_MAX_SIZE
 * Q_MAX_SIZE: Capacity of the buffer.
 *********************************************************************************************************/
size_t cbfifo_capacity(Queue_T *q)
{
    return Q_MAX_SIZE;
}

/*******************************************************************************************************
 * Function to enqueue data in cbfifo.
 * Function: cbfifo_enqueue()
 * Parameters: - Queue_T *q, void* buf, size_t nbyte
 * *q: Pointer to the structure
 * *buf: Pointer to the data to be enqueued.
 * nbyte: Number of elements to be enqueued.
 * Returns: - size_t count
 * count: Number of elements enqueued.
 *********************************************************************************************************/
size_t cbfifo_enqueue(Queue_T * q, void* buf, size_t nbyte)
{
	uint32_t masking_state;
	masking_state = __get_PRIMASK();		/*save current masking state*/
	__disable_irq();						/*global disable IRQs*/
	if(q->full)
	{
		return 0;
	}
	char *src = (char*)buf;                 /*src stores the address in the Buf */
	size_t count = 0;

	while(count < nbyte)
	{
		if(q->full)                         /*if buffer is full then break*/
		{
			break;
		}

		else
		{
			q->Data[q->Rear] = *src;         /*Store the byte of data in the buffer*/
		    count++;
		    src++;
		    if(q->Rear == Q_MAX_SIZE - 1)	/*Check if the index reaches to the last position of the buffer*/
		    {
		    	q->Rear = 0;
		    }
		    else
		    {
		    	q->Rear = q->Rear + 1;		/*Increment q->Rear by one*/
		    }
		    if(q->Rear == q->Front)			/*Check if the buffer is full*/
		    {
		    	q->full = 1;
		    }
		}
	}
	q->Size = q->Size + count;				/*Update total number of elements in the buffer*/
	__set_PRIMASK(masking_state);			/*restore  interrupt masking state*/
	__enable_irq();							/*global enable IRQs*/
	return count;
}

/*******************************************************************************************************
 * Function to dequeue data in cbfifo.
 * Function: cbfifo_dequeue()
 * Parameters: - Queue_T *q, void* buf, size_t nbyte
 * *q: Pointer to the structure
 * *buf: Pointer to the data that has been dequeued.
 * nbyte: Number of elements to be dequeued.
 * Returns: - size_t count
 * count: Number of elements dequeued.
 *********************************************************************************************************/
size_t cbfifo_dequeue(Queue_T *q, void *buf, size_t nbyte)
{
	uint32_t masking_state;
	masking_state = __get_PRIMASK();		/*save current masking state*/
	__disable_irq();						/*global disable IRQs*/
	char *dec_buf = (char*)buf;             /*dec stores the address in the Buf */
	size_t count = 0;

	while(count < nbyte)
	{
		if(Queue_Empty(q))					/*Check if the buffer is empty*/
		{
			q->Front = 0;
			q->Rear = 0;
			break;
		}

		else
		{
			*dec_buf = q->Data[q->Front]; 	/*Dequeue data from the buffer*/
	        dec_buf++;
	        count++;
	        if(q->Front == Q_MAX_SIZE - 1)	/*Check if the index reaches to the last position of the buffer*/
	        {
	        	q->Front = 0;
	        }
	        else
	        {
	        	q->Front = q->Front + 1;	/*Increment q->Front by one*/
	        }
	        q->full = 0;
		}
	}

	q->Size = q->Size - count;				/*Update total number of elements in the buffer*/
	__set_PRIMASK(masking_state);			/*restore  interrupt masking state*/
	__enable_irq();							/*global enable IRQs*/
	return count;
}
