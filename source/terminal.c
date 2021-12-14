/**************************************************************************************************************************
 * File_name : - terminal.c
 * Description: - Initializes cbfifo and contains functions for enqueuing and dequeuing data into cbfifo.
 * Author : - Darshit Nareshkumar Agrawal
 * Compiler: - MCU C Compiler
 * Source/ Snippet references: - 1) Freescale Arm Cortex-M Embedded Programming by Mazidi &
 *                               Naimi
 *2) Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers.
 *3) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/I2C-Demo
 *4) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 ***************************************************************************************************************************/

#include "MKL25Z4.h"										/*Header files*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "uart.h"
#include "queue.h"
#include "terminal.h"
#include "mma8451.h"
#include "LEDs.h"
#include "delay.h"
#include <math.h>

#define PWM_STEP_SIZE (1.41666)								/*Macros*/

extern float calibrated_angle;
extern unsigned int break_flag,one_time_flag;

typedef void (*command_handler_t)(int, char *argv[]);

typedef struct												/*Structure declaration*/
{
	const char *name;
	command_handler_t handler;
}command_table_t;

/*******************************************************************************************************
 * Function to display relevant messages when author command is given by the user.
 * Function: author()
 * Parameters: - int argc, char* argv[]
 * argc: Number of arguments
 * *argv[]: Individual argument passed
 * Returns: - None
 *********************************************************************************************************/
void author(int argc, char *argv[])
{
	if(argc != 1)											/*Print message if unknown command is given*/
	{
		printf("Unknown command:");
		for(int count = 0;count < argc;count++)
		{
			printf("%s ",argv[count]);
		}
		printf("\n\r");
		return;
	}
	printf("Darshit Nareshkumar Agrawal\n\r");				/*Print author's name*/
}

/*******************************************************************************************************
 * Function to display angle in degree every one second along with change in tricolor LED.
 * Function: input()
 * Parameters: - int argc, char* argv[]
 * argc: Number of arguments
 * *argv[]: Individual argument passed
 * Returns: - None
 *********************************************************************************************************/
void input(int argc, char *argv[])
{
	uint32_t angle;
	float tilt_angle, final_angle;

	if(argc != 2)											/*Print message if unknown command is given*/
	{
		printf("Unknown command: ");
		for(int count = 0;count < argc;count++)
		{
			printf("%s ",argv[count]);
		}
		printf("\n\r");
		return;
	}														/*Extract starting address and length inputted by the user from the string*/
	if((sscanf(argv[1],"%d",&angle) == 1))
	{
		while(1)
		{
			if(break_flag)
			{
				read_full_xyz();
				convert_xyz_to_roll_pitch();
				tilt_angle = fabs(tilt);
				final_angle = tilt_angle - calibrated_angle;
				if(calibrated_angle != 0 && one_time_flag == 1)
				{
					printf("\n\rAngle has been calibrated at %f",calibrated_angle);
					one_time_flag = 0;
				}
				if(final_angle < -1)
				{
					Control_RGB_LEDs(0, 0, 0);
				}

				else if(final_angle < angle && final_angle > 1)
				{
					Control_RGB_LEDs((int)PWM_STEP_SIZE*final_angle, 0, 0);
				}
				else if(final_angle == angle)
				{
					Control_RGB_LEDs(255,0,0);
				}
				else if(final_angle > angle)
				{
					Control_RGB_LEDs(0, (int)PWM_STEP_SIZE*final_angle, 0);
				}
				else
				{
					Control_RGB_LEDs(255, 255, 255);
				}
				one_sec_delay();
				printf("\n\rCurrent Angle in degrees = %f",final_angle);
			}
			else
			{
				break;
			}
		}
		calibrated_angle = 0;
		break_flag = 1;
	}
}

/*******************************************************************************************************
 * Function to display relevant messages when help command is given by the user.
 * Function: help()
 * Parameters: - int argc, char* argv[]
 * argc: Number of arguments
 * *argv[]: Individual argument passed
 * Returns: - None
 *********************************************************************************************************/
void help(int argc, char *argv[])
{
	if(argc != 1)
	{
		printf("Unknown command: ");						/*Print message if unknown command is given*/
		for(int count = 0;count < argc;count++)
		{
			printf("%s ",argv[count]);
		}
		printf("\n\r");
		return;
	}
	printf("author");										/*Print messages with information on various commands*/
	printf("\n\r\tPrint the author of this code.");
	printf("\n\rinput <angle>");
	printf("\n\r\tStart the digital angle gauge and print angle values on UART. Monitor LED color for angle");
	printf("\n\rhelp");
	printf("\n\r\tPrint this help message");
	printf("\n\r");
}

/*******************************************************************************************************
 * Function to display relevant messages when some unknown command is given by the user.
 * Function: unknown_command()
 * Parameters: - int argc, char* argv[]
 * argc: Number of arguments
 * *argv[]: Individual argument passed
 * Returns: - None
 *********************************************************************************************************/
void unknown_command(int argc, char* argv[])
{
	printf("Unknown command : ");							/*Print message if unknown command is given*/
	for(int count = 0;count < argc;count++)
	{
		printf("%s ",argv[count]);
	}
		printf("\n\r");
}

static const command_table_t commands[] = {					/*Command table*/
											{"author",author},
											{"help",help},
											{"input",input}
										  };

static const int num_commands = sizeof(commands) / sizeof(command_table_t);

/*******************************************************************************************************
 * Function to take commands from the user until carriage return or line feed is detected.
 * Function: terminal_input()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/
void terminal_input()
{
	char input_buffer[640];									/*Array to store the bytes inputted by the user*/
	char *input;
	input = input_buffer;
	char c;
	int count = 0;
	while(c != '\n' || c != '\r')
	{
		while(Queue_Size(&receive_t) == 0)					/*Wait till there is an element in the receive buffer*/
				;
		cbfifo_dequeue(&receive_t, &c, 1);					/*Dequeue element from the receive buffer*/
		printf("%c",c);										/*Print the dequeued element*/

		if(c != '\n' && c != '\r' && c != '\b')				/*Keep taking bytes until line feed or carriage return or backspace is detected*/
		{
			*(input + count) = c;
			count++;
		}

		if(c == '\b' && count >= 0)							/*Replace the character with ' ' when backspace is detected*/
		{
			printf(" ");
			printf("\b");
			count--;
		}

		if(c == '\n' || c == '\r')							/*Terminate the string with Null when line feed or carriage return is detected*/
		{
			*(input + count) = '\0';
			break;
		}
	}
	printf("\n\r");											/*Display next message on next line*/
	lexical_analysis(input);
}

/*******************************************************************************************************
 * Function to process commands given by the user.
 * Function: lexical_analysis()
 * Parameters: - char* input
 * *input: Input command given by the user.
 * Returns: - None
 *********************************************************************************************************/
void lexical_analysis(char *input)
{
	int char_detected = 0;
	int argc = 0;
	char *argv[10];
	char *temp = NULL;
	temp = input;
	memset(argv, 0, sizeof(argv));							/*Assign 0 to argv*/
	while(*temp != '\0')
	{
		if(*temp != ' ' && char_detected == 0)				/*Store the string in argv[argc] if the first byte is not space*/
		{
			argv[argc] = temp;
			char_detected = 1;
			argc++;
		}
		else if(*temp == ' ' && char_detected == 1)			/*Terminate argv[argc] with '\0' if space is detected*/
		{
			char_detected = 0;
			*temp = '\0';
		}
		temp++;
	}
	printf("\n\r");											/*Go to the next line*/

	if(argc == 0)											/*return if no useful byte is given other than space*/
	{
		return;
	}

	for(int i = 0;i < num_commands;i++)
	{														/*compare the input string with commands in the look-up table and call appropriate functions*/
		if(strcasecmp(argv[0],commands[i].name) == 0)
		{
			commands[i].handler(argc,argv);
			return;
		}
	}
	unknown_command(argc, argv);							/*Call unknwon command function if commands does not match with look-up table*/
}

/*******************************************************************************************************
 * Function to display message at the start of the Terminal.
 * Function: Terminal()
 * Parameters: - None
 * Returns: - None
 *********************************************************************************************************/
void Terminal(void)
{
	printf("\n\rWelcome to Digital Angle Gauge!");			/*Print the welcome message*/

	while(1)
	{
		Control_RGB_LEDs(0,0,0);
		printf("\n\r? ");
		terminal_input();
	}
}
