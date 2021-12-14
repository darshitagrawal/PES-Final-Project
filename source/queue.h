/**************************************************************************************************************************
 * File_name : - queue.h
 * Description: - Contains prototypes for cbfifo operation.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/
#ifndef QUEUE_H_
#define QUEUE_H_

#include <MKL25Z4.h>					/*Header files*/
#include <stdbool.h>

#define Q_MAX_SIZE (256)

typedef struct {
  int Front; 							/*Index of oldest data element*/
  int Rear; 							/*Index of next free space*/
  int Size; 							/*Number of elements in use*/
  bool full;							/*Flag to check if cbfifo is full*/
  char Data[Q_MAX_SIZE];				/*Array to store data*/
}  Queue_T;

extern Queue_T transmit_t, receive_t;	/*Global structure variables*/

/*******************************************************************************************************
 * Function to check whether the cbfifo is empty or not and returns relevant value as per the status.
 * Function: Queue_Empty
 * Parameters: - Queue_T *q
 * *q = Pointer to the structure
 * Returns: - bool
 *********************************************************************************************************/

bool Queue_Empty(Queue_T * q);

/*******************************************************************************************************
 * Function to check whether the cbfifo is full or not and returns relevant value as per the status.
 * Function: Queue_Full
 * Parameters: - Queue_T *q
 * *q: Pointer to the structure
 * Returns: - bool
 *********************************************************************************************************/

bool Queue_Full(Queue_T * q);

/*******************************************************************************************************
 * Function to return the number of elements currently in the cbfifo.
 * Function: Queue_Size
 * Parameters: - Queue_T *q
 * *q: Pointer to the structure
 * Returns: - int q->size
 * q->size: Size of the structure
 *********************************************************************************************************/

int Queue_Size(Queue_T * q);

/*******************************************************************************************************
 * Function to return the number of elements currently in the cbfifo.
 * Function: cbfifo_length()
 * Parameters: - Queue_T *q
 * *q: Pointer to the structure
 * Returns: - size_t q->Size
 * q->Size: Size of the structure
 *********************************************************************************************************/

size_t cbfifo_length(Queue_T *q);

/*******************************************************************************************************
 * Function to return the total capacity of the cbfifo.
 * Function: cbfifo_capacity()
 * Parameters: - Queue_T *q
 * *q: Pointer to the structure
 * Returns: - size_t Q_MAX_SIZE
 * Q_MAX_SIZE: Capacity of the buffer.
 *********************************************************************************************************/

size_t cbfifo_capacity(Queue_T *q);

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

size_t cbfifo_enqueue(Queue_T * q, void* buf, size_t nbyte);

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

size_t cbfifo_dequeue(Queue_T *q, void *buf, size_t nbyte);

/*******************************************************************************************************
 * Initializes cbfifo at the beginning of enqueuing or dequeuing data.
 * Function: Queue_Init()
 * Parameters: - Queue_T *q
 * *q = Pointer to the structure to be intialized.
 * Returns: - None
 *********************************************************************************************************/

void Queue_Init(Queue_T * q);

#endif /* QUEUE_H_ */
